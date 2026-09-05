# Frame-depth overlap experiment

Branch: `perf/frame-depth-overlap`.

The renderer allocates one depth attachment per frame slot by default. The slot's
existing fence is waited before reusing its attachment. Since depth is cleared
every frame and discarded after rendering, its transition starts from UNDEFINED
with an empty source stage/access scope. This removes the shared depth attachment's
cross-frame early/late fragment-test dependency. At two frames in flight this uses
one additional depth image (about 2 MiB at 960x540 with D32).

`--shared-depth` restores the original single attachment and its dependency for
comparison. The default presentation preference remains Mailbox, Immediate, FIFO.
Explicit presentation requests fail if unsupported, rather than silently changing
the benchmark configuration. The runtime validation flag now enables both the
validation layer and debug-utils extension.

## Run comparisons

Build Release using the project's normal build configuration. From the build
directory on macOS (use `apps/sandbox/sandbox.exe` on Windows):

```sh
./apps/sandbox/sandbox --benchmark-seconds=10 --shared-depth
./apps/sandbox/sandbox --benchmark-seconds=10
./apps/sandbox/sandbox --benchmark-seconds=10 --shared-depth --present=immediate
./apps/sandbox/sandbox --benchmark-seconds=10 --present=immediate
```

Repeat in reverse order. Add `--no-model` to compare clear-and-present workloads.
Use `--present=mailbox` or `--present=fifo` to request those modes explicitly.
Do not resize, minimize, or obscure the window during a measured run. Compare the
same actual pixel dimensions, driver, scene, and presentation mode. Startup logs
identify the selected presentation mode, extent, and depth attachment count.

Each benchmark warms up for three seconds after scene initialization, then measures
the requested duration, drains submitted GPU work, prints a BENCHMARK summary, and
exits. CPU sections are elapsed wall time including waits; they are not GPU
timestamps. Overall FPS includes the final GPU drain and periodic log overhead.
Normal interactive runs remain available by omitting `--benchmark-seconds`.

For a separate correctness run on macOS:

```sh
VK_LAYER_VALIDATE_SYNC=1 ./apps/sandbox/sandbox --validation --benchmark-seconds=5
```

For PowerShell:

```powershell
$env:VK_LAYER_VALIDATE_SYNC = '1'
./apps/sandbox/sandbox.exe --validation --benchmark-seconds=5
Remove-Item Env:VK_LAYER_VALIDATE_SYNC
```

Disable validation during performance measurements. macOS/KosmicKrisp performance
must be measured on the Mac; Windows results do not establish the macOS bottleneck.

## Local Windows results (2026-09-05)

MSVC Release, Vulkan-Hpp modules enabled, RTX 3070 Laptop GPU, 960x540, one model,
two frames in flight, three requested swapchain images. Each row contains two
eight-second runs after three seconds of warmup. For each presentation mode,
the run order was shared, per-frame, per-frame, shared. Validation was disabled.

| Presentation | Depth | FPS runs | Fence ms/frame | Acquire ms/frame |
| --- | --- | --- | --- | --- |
| Mailbox | Shared | 1797, 1502 | 0.019, 0.021 | 0.350, 0.445 |
| Mailbox | Per-frame | 1599, 1623 | 0.021, 0.019 | 0.409, 0.404 |
| Immediate | Shared | 1647, 1603 | 0.407, 0.424 | 0.007, 0.007 |
| Immediate | Per-frame | 1671, 1729 | 0.410, 0.392 | 0.007, 0.007 |

There is no convincing overall depth-optimization win in these short runs.
Immediate per-frame depth averaged about 4.6% above shared depth, while Mailbox
averaged about 2.4% below it; run-to-run variation limits the conclusion. Switching
to Immediate mainly moved time from acquisition into fence waiting, without a
large throughput improvement. A short empty-scene check with per-frame depth gave
1817 FPS in Mailbox and 1598 FPS in Immediate, consistent with substantial fixed
per-frame overhead.

Release build and diff whitespace checks passed. Separate synchronization
validation runs passed for per-frame Mailbox, per-frame Immediate, and shared
Mailbox. The layer reported an existing unused vertex attribute at location 1;
no validation errors or synchronization hazards were reported. macOS has not
been tested in this session.
