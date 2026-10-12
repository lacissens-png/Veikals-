// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Combat/EclipseDamageExecution.h"

#include "Combat/EclipseAttributeSet.h"
#include "EclipseCore/DamageMath.h"
#include "EclipseGameplayTags.h"

namespace
{
	struct FEclipseDamageStatics
	{
		FGameplayEffectAttributeCaptureDefinition ArmorDef;

		FEclipseDamageStatics()
		{
			// Snapshot = false: use the target's armor at the moment of execution.
			ArmorDef = FGameplayEffectAttributeCaptureDefinition(UEclipseAttributeSet::GetArmorAttribute(),
				EGameplayEffectAttributeCaptureSource::Target, false);
		}
	};

	const FEclipseDamageStatics& DamageStatics()
	{
		static FEclipseDamageStatics Statics;
		return Statics;
	}
}

UEclipseDamageExecution::UEclipseDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UEclipseDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvalParams, Armor);

	const float BaseDamage = Spec.GetSetByCallerMagnitude(EclipseTags::Data_Damage, false, 0.f);
	const float ArmorPen = Spec.GetSetByCallerMagnitude(EclipseTags::Data_ArmorPenetration, false, 0.f);

	// DamageTakenMultiplier is applied later in the AttributeSet (status-aware), so pass 1 here.
	const float Final = Eclipse::ComputeFinalDamage(BaseDamage, Armor, ArmorPen, 1.f);
	if (Final > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(UEclipseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Final));
	}
}
