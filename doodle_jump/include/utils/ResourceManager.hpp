#pragma once
#include <map>
#include <string>

using namespace std;

template <typename T>
class ResourceManager {
public:
    T& load(const string& id, const string& path)
    {
        auto it = cache.find(id);

        if (it == cache.end())
        {
            T asset;

            if (!asset.loadFromFile(path))
            {
                throw runtime_error("Cannot load: " + path);
            }

            it = cache.emplace(id, std::move(asset)).first;
        }

        return it->second;
    }

    T& get(const string& id) {
        return cache.at(id);
    }
    
private:
    map<string, T> cache;
};