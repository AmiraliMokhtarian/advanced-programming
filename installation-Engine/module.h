#pragma once
#include "installable.h"

class module : public installable{
public:
    module(string id, string title);
    bool install(transactionContext& tx) override;
};