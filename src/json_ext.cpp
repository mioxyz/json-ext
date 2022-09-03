#include <nlohmann/json_fwd.hpp>
#include "nlohmann/json.hpp"
#include <functional>
#include <vector>
#include <stdexcept>
namespace nlohmann {
   namespace ext {

      struct BooleanPartition 
      {
         std::vector<json::iterator> positive;
         std::vector<json::iterator> negative;

         json buildPositiveJson() {
            json arr;
            for(auto& j : positive) arr.push_back(*j);
            return arr;
         }

         json buildNegativeJson() {
            json arr;
            for(auto& j : negative) arr.push_back(*j);
            return arr;
         }

         std::string dumpPositive(int indentation = 3) {
            std::stringstream ss;
            ss << "[";
            for(auto& j : positive) ss << "\n" << j->dump(indentation);
            ss << "\n" << "]";
            return ss.str();
         }

         std::string dumpNegative(int indentation = 3) {
            std::stringstream ss;
            for(auto& j : negative) ss << "\n" << j->dump(indentation);
            return ss.str();
         }
      };

      json::iterator find(nlohmann::json& arr, std::function<bool(nlohmann::json::iterator)> f) {
         if(!arr.is_array())
            throw std::invalid_argument("object must be of type nlohmann::json::array_t");

         for(json::iterator itr = arr.begin(); itr != arr.end(); ++itr)
            if(f(itr))
               return itr;

         return arr.end();
      }

      bool contains(nlohmann::json& arr, std::function<bool(nlohmann::json::iterator)> f) {
         return ext::find(arr, f) != arr.end();
      }

      bool excludes(nlohmann::json& arr, std::function<bool(nlohmann::json::iterator)> f) {
         return ext::find(arr, f) == arr.end();
      }

      std::vector<json::iterator> findAll(nlohmann::json& arr, std::function<bool(json::iterator)> f) {
         std::vector<json::iterator> resultSet;

         if(!arr.is_array())
            throw std::invalid_argument("object must be of type nlohmann::json::array_t");

         for(auto itr = arr.begin(); itr != arr.end(); ++itr)
            if(f(itr)) resultSet.push_back(itr);

         return resultSet;
      }

      ext::BooleanPartition partition(nlohmann::json& arr, std::function<bool(json::iterator)> f) {
         BooleanPartition part;

         if(!arr.is_array())
            throw std::invalid_argument("object must be of type nlohmann::json::array_t");

         for(auto itr = arr.begin(); itr != arr.end(); ++itr)
            if(f(itr))
               part.positive.push_back(itr);
            else
               part.negative.push_back(itr);

         return part;
      }

      bool some(nlohmann::json& arr, std::function<bool(json::iterator)> f) {
         for(json::iterator itr = arr.begin(); itr != arr.end(); ++itr)
            if(f(itr)) return true;
         return false;
      }

      bool none(nlohmann::json& arr, std::function<bool(json::iterator)> f) {
         return !some(arr, f);
      }

      std::vector<json::iterator> intersection(json& A, json& B) {
         if(!A.is_array()) throw std::invalid_argument("object A must be of type nlohmann::json::array_t");
         if(!B.is_array()) throw std::invalid_argument("object B must be of type nlohmann::json::array_t");

         std::vector<json::iterator> intersect;

         for(auto elem = A.begin(); elem!=A.end(); ++elem)
            if(nlohmann::ext::contains(B, [elem](json::iterator other) -> bool { return *other == *elem; }))
               intersect.push_back(elem);

         return std::move(intersect);
      }

      void filter(json& arr, std::function<bool(json::iterator)> f) {
         if(!arr.is_array())
            throw std::invalid_argument("object must be of type nlohmann::json::array_t");

         auto itr = arr.begin();
         for(; itr != arr.end();) {
            if(!f(itr)) {
               arr.erase(itr);
            } else {
               ++itr;
            }
         }
      }

      // Mutates A in-place & hard
      void append(json& arr, json& appendage) {
         for(auto itr= appendage.begin(); itr != appendage.end(); ++itr)
            arr.push_back(*itr);
      }

   }
}
