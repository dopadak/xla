/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef XLA_SERVICE_GPU_FUSIONS_LOOP_H_
#define XLA_SERVICE_GPU_FUSIONS_LOOP_H_

#include <vector>

#include "xla/hlo/ir/hlo_instructions.h"
#include "xla/mlir_hlo/lhlo/IR/lhlo_ops.h"
#include "xla/service/elemental_ir_emitter.h"
#include "xla/service/gpu/fusions/fusion_emitter.h"
#include "xla/service/gpu/hlo_fusion_analysis.h"
#include "xla/service/gpu/ir_emitter_context.h"

namespace xla {
namespace gpu {

// Generic loop fusion.
class LoopFusion : public KernelFusionEmitterBase {
 public:
  LoopFusion(IrEmitterContext& ir_emitter_context,
             ElementalIrEmitter& elemental_emitter,
             mlir::lmhlo::FusionOp fusion_op,
             const HloFusionInstruction& fusion, HloFusionAnalysis& analysis)
      : KernelFusionEmitterBase(ir_emitter_context, elemental_emitter,
                                fusion_op, fusion),
        analysis_(analysis) {}
  StatusOr<LaunchDimensions> launch_dimensions() const override {
    return analysis_.GetLaunchDimensions();
  }

 protected:
  Status EmitKernel(const KernelArguments& args,
                    const LaunchDimensions& launch_dims,
                    std::vector<llvm_ir::IrArray> ir_arrays,
                    llvm::IRBuilder<>* builder) const override;

 private:
  HloFusionAnalysis& analysis_;
};

}  // namespace gpu
}  // namespace xla

#endif  // XLA_SERVICE_GPU_FUSIONS_LOOP_H_
