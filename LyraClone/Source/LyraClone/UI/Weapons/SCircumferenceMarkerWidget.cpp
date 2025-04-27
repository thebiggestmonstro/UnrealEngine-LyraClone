// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/SCircumferenceMarkerWidget.h"
#include "Engine/UserInterfaceSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SCircumferenceMarkerWidget)

SCircumferenceMarkerWidget::SCircumferenceMarkerWidget()
{

}

void SCircumferenceMarkerWidget::Construct(const FArguments& InArgs)
{
	MarkerBrush = InArgs._MarkerBrush;
	MarkerList = InArgs._MarkerList;
	Radius = InArgs._Radius;
	bColorAndOpacitySet = InArgs._ColorAndOpacity.IsSet();
	ColorAndOpacity = InArgs._ColorAndOpacity;
}

void SCircumferenceMarkerWidget::SetRadius(float NewRadius)
{
	if (Radius.IsBound() || (Radius.Get() != NewRadius))
	{
		Radius = NewRadius;
		Invalidate(EInvalidateWidgetReason::Layout);
	}
}

void SCircumferenceMarkerWidget::SetMarkerList(TArray<FCircumferenceMarkerEntry>& NewMarkerList)
{
	MarkerList = NewMarkerList;
}

FSlateRenderTransform SCircumferenceMarkerWidget::GetMarkerRenderTransform(const FCircumferenceMarkerEntry& Marker, const float BaseRadius, const float HUDScale) const
{
	float XRadius = BaseRadius;
	float YRadius = BaseRadius;
	if (bRecticleCornerOutsideSpreadRadius)
	{
		// Image 사이즈의 반만큼 (Radius) 더해줘서 원 바깥에서 마커가 렌더링되도록 한다
		XRadius += MarkerBrush->ImageSize.X * 0.5f;
		YRadius += MarkerBrush->ImageSize.Y * 0.5f;
	}

	// degree -> radians
	const float LocalRotationRadians = FMath::DegreesToRadians(Marker.ImageRotationAngle);
	const float PositionAngleRadians = FMath::DegreesToRadians(Marker.PositionAngle);

	FSlateRenderTransform RotateAboutOrigin(
		// RotateAboutOrigin은 ImageSize/2.0만큼 옮기고 회전시키고 다시 옮겨놓는다
		Concatenate(
			FVector2D(-MarkerBrush->ImageSize.X * 0.5f, -MarkerBrush->ImageSize.Y * 0.5f),
			FQuat2D(LocalRotationRadians),
			FVector2D(MarkerBrush->ImageSize.X * 0.5f, MarkerBrush->ImageSize.Y * 0.5f)
		)
	);

	// 회전한 image를 circumference 방식으로 위치를 translation 시켜준다
	return TransformCast<FSlateRenderTransform>(
		Concatenate(
			RotateAboutOrigin,
			FVector2D(XRadius * FMath::Sin(PositionAngleRadians) * HUDScale, -YRadius * FMath::Cos(PositionAngleRadians) * HUDScale)
		)
	);
}

int32 SCircumferenceMarkerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	// GetLocalPositionAtCoordinate(2D(0.5, 0.5)) == 중심좌표가 나온다
	const FVector2D LocalCenter = AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f));

	const bool bDrawMarkers = (MarkerList.Num() > 0) && (MarkerBrush != nullptr);
	if (bDrawMarkers)
	{
		const FLinearColor MarkerColor = bColorAndOpacitySet ?
			ColorAndOpacity.Get().GetColor(InWidgetStyle) :
			(InWidgetStyle.GetColorAndOpacityTint() * MarkerBrush->GetTint(InWidgetStyle));

		// 아래의 공식은 숙제로 한번 유도해보자
		if (MarkerColor.A > KINDA_SMALL_NUMBER)
		{
			const float BaseRadius = Radius.Get();
			const float ApplicationScale = GetDefault<UUserInterfaceSettings>()->ApplicationScale;
			for (const FCircumferenceMarkerEntry& Marker : MarkerList)
			{
				const FSlateRenderTransform MarkerTransform = GetMarkerRenderTransform(Marker, BaseRadius, ApplicationScale);
				const FPaintGeometry Geometry(
					AllottedGeometry.ToPaintGeometry(MarkerBrush->ImageSize, FSlateLayoutTransform(LocalCenter - (MarkerBrush->ImageSize * 0.5f)),
						MarkerTransform,
						FVector2D(0.0f, 0.0f))
				);
				FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geometry, MarkerBrush, DrawEffects, MarkerColor);
			}
		}
	}

	return LayerId;
}

FVector2D SCircumferenceMarkerWidget::ComputeDesiredSize(float) const
{
	// ComputeDesiredSize()는 Immediate 구조 UI 프레임워크에서는 매 프레임 불린다:
	// - Retained vs Immediate
	check(MarkerBrush);
	const float SampledRadius = Radius.Get();
	return FVector2D((MarkerBrush->ImageSize.X + SampledRadius) * 2.0f, (MarkerBrush->ImageSize.Y + SampledRadius) * 2.0f);
}
