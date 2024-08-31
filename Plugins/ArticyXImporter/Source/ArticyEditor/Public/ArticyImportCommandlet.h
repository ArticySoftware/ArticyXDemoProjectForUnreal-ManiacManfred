#pragma once

#include "Commandlets/Commandlet.h"
#include "ArticyImportCommandlet.generated.h"

/**
 * UArticyImportCommandlet is a custom commandlet class used for importing Articy data.
 */
UCLASS()
class UArticyImportCommandlet : public UCommandlet
{
    GENERATED_BODY()

    /**
     * Main function that processes command line arguments and performs import operations.
     *
     * @param Params Command line parameters passed to the commandlet.
     * @return int32 Status code representing the result of the commandlet execution.
     */
    virtual int32 Main(const FString& Params) override;
};
