[[vk::binding(0, 0)]]
Texture2D<float4> textures[MAX_TEXTURES];

[[vk::binding(1, 0)]]
SamplerState samplers[MAX_SAMPLERS];

[[vk::binding(0, 1)]]
ConstantBuffer<FrameUniformBuffer> frame;

struct FrameUniformBuffer {
    float4x4 view, projection, view_projection;
    float4x4 inverse_view, inverse_projection;
    float3 camera_position;
    float padding;
    float2 resolution;
    float2 mouse;
    float time;
    float delta_time;
    uint64_t lights;
}

struct TextureDrawData
{
    uint64_t texture_index;
    uint64_t samplerIndex;
    float2 size;
};

struct PushConstants {
    TextureDrawData* instances;    // base address
};
struct VSOutput
{
    float4 pos : SV_Position;
    float3 fragColor;
    float2 fragTexCoord;
    nointerpolation uint instanceIndex;
};
[[vk::push_constant]]
ConstantBuffer<PushConstants> push_constants;
struct VSInput {
    [[vk::location(0)]] float3 position;
    [[vk::location(1)]] float3 normal;
    [[vk::location(2)]] float2 uv;
};


[shader("vertex")]
VSOutput vertMain(uint id : SV_VertexID) {
    float2 uv = float2((id << 1) & 2, id & 2);
    VSOutput o;
    o.fragTexCoord = uv; 
    o.pos = float4(uv * 2 - 1, 0, 1); 
    return o;
}
[shader("fragment")]
float4 fragMain(VSOutput i) : SV_Target {
    let d = push_constants.instances[0];
    return textures[d.texture_index].Sample(samplers[d.samplerIndex], i.fragTexCoord);
}