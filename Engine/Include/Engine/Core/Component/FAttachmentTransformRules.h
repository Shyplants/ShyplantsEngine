#pragma once

struct FAttachmentTransformRules
{
    bool bMaintainWorld = true;

    static const FAttachmentTransformRules KeepWorldTransform;
    static const FAttachmentTransformRules KeepRelativeTransform;
};

struct FDetachmentTransformRules
{
    bool bMaintainWorld = true;

    static const FDetachmentTransformRules KeepWorldTransform;
    static const FDetachmentTransformRules KeepRelativeTransform;
};