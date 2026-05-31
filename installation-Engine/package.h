#pragma once
#include "installable.h"

using namespace std;

class package : public installable{
private:
    vector<installable*> children;
public:
    package(string id, string title);
    bool isPackage() const override { return true;}
    void addChild(installable* child);
    bool hasChild(const string& child_id) const;
    bool install(transactionContext& tx) override;
    void unInstall() override;
};