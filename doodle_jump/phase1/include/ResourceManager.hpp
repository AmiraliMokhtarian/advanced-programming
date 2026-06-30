#pragma once
#include <map>
#include <string>

using namespace std;

template <typename T>
class ResourceManager {
public:
    T& load(const string& id, const string& path) {

        if (cache.find(id) == cache.end()){
            T asset;
            asset.loadFromFile(path);

            cache[id] = move(asset);
        }

        return cache[id];
    }

    T& get(const string& id) {
        return cache[id];
    }
    
private:
    map<string, T> cache;
};