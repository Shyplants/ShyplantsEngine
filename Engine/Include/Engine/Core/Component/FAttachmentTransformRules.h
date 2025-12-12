#pragma once

struct FAttachmentTransformRules
{
    bool bMaintainWorld = false;

    static const FAttachmentTransformRules KeepWorldTransform;
    static const FAttachmentTransformRules KeepRelativeTransform;
};

struct FDetachmentTransformRules
{
    bool bMaintainWorld = false;

    static const FDetachmentTransformRules KeepWorldTransform;
    static const FDetachmentTransformRules KeepRelativeTransform;
};