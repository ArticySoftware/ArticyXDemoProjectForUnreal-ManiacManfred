#pragma once

#include "Commandlets/Commandlet.h"
#include "ArticyImportCommandlet.generated.h"

UCLASS()
class UArticyImportCommandlet : public UCommandlet
{
    GENERATED_BODY()

    virtual int32 Main(const FString& Params) override;
};
