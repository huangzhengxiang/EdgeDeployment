## -ffast-math does a lot more than just break strict IEEE compliance.

- First of all, of course, it does break strict IEEE compliance, allowing e.g. the reordering of instructions to something which is mathematically the same (ideally) but not exactly the same in floating point.

- Second, it disables setting errno after single-instruction math functions, which means avoiding a write to a thread-local variable (this can make a 100% difference for those functions on some architectures).

- Third, it makes the assumption that all math is finite, which means that no checks for NaN (or zero) are made in place where they would have detrimental effects. It is simply assumed that this isn't going to happen.

- Fourth, it enables reciprocal approximations for division and reciprocal square root.

- Further, it disables signed zero (code assumes signed zero does not exist, even if the target supports it) and rounding math, which enables among other things constant folding at compile-time.

- Last, it generates code that assumes that no hardware interrupts can happen due to signalling/trapping math (that is, if these cannot be disabled on the target architecture and consequently do happen, they will not be handled).

Possibly safe optimization flags in `-Ofast` are: 

- `-fallow-store-data-races` for a <b>non-multi-threading device</b>.
- `-fno-math-errno` for programs do not need math <b>error handling</b>.
- `-ffinite-math-only` for programs ensuring that results are <b>not NaNs or +-Infs</b>.
- `-fno-signaling-nans`
- `-fcx-limited-range` for programs without complex number calculation.
- `-funsafe-math-optimizations`: containing everything below and severely enlarges fp precision to 10^{-8}*operand.
- `-fno-signed-zeros`, `-fno-trapping-math`, `-fno-rounding-math`,`-fassociative-math`: may cause overflow / underflow.
- `-freciprocal-math`: ?