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

struct DrawData
{
    float4x4 model;
    uint64_t textureIndex;
    uint64_t samplerIndex;
};

[[vk::push_constant]]
ConstantBuffer<DrawData> draw;
struct VSInput {
    [[vk::location(0)]] float3 position;
    [[vk::location(1)]] float3 normal;
    [[vk::location(2)]] float2 uv;
};
struct VSOutput
{
    float4 pos : SV_Position;
    float3 fragColor;
    float2 fragTexCoord;
};

[shader("vertex")]
VSOutput vertMain(VSInput input) {
    VSOutput output;
    output.pos = mul(frame.projection, mul(frame.view, mul(draw.model, float4(input.position, 1.0))));
    //output.fragColor = input.inColor;
    output.fragTexCoord = input.uv;
    return output;
}



[shader("fragment")]
float4 fragMain(VSOutput vertIn) : SV_TARGET {
   return float4(textures[draw.textureIndex].Sample(samplers[draw.samplerIndex], vertIn.fragTexCoord).rgb, 1.0);
}