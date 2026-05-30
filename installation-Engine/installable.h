#pragma once
#include <string>
#include <vector>
#include "ComponentState.h"
#include "observer.h"


struct transactionContext
{
    vector<installable*> stateChangedNodes;
    vector<installable*> countIncreasedNodes;
};


class installable{
protected:
    string id;
    string title;
    componentState state;
    bool mockFail;
    
    int installedParentCount;
    bool is_explicitly_installed;
    vector<observer*> observers;

public:
    installable(string _id_, string _title_);
    virtual ~installable() {}
    string getId () const {return id;} 
    string getTitle () const {return title;}
    componentState getState () const {return state;}
    bool isMockFail() const {return mockFail;}
    void setMockFail(bool val) {mockFail = val;};
    void setState(componentState new_state);
    void incParents();
    void decParents();
    bool getExplicit() const {return is_explicitly_installed;}
    void setExplicit(bool val) {is_explicitly_installed = val;}
    int getParentCount() const {return installedParentCount;}
    void forcePending();
    void addObservers(observer* obs);
    virtual bool isPackage() const { return false;}
    virtual bool install(transactionContext& tx) = 0;
    virtual void unInstall() = 0;
};