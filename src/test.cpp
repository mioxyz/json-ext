#include <nlohmann/json_fwd.hpp>
#include "nlohmann/json.hpp"
#include <functional>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include "json_ext.cpp"

using namespace nlohmann;

void addPeople(json& arr, int amount = 3) {
   for(int k=0; k < amount; ++k) {
      arr.push_back({
         { "name", "Jason" },
         { "age", 19+k },
         { "favorite_food", "pizza" }
      });
   }
}

void unitTest_find() {
   json people;
   addPeople(people);

   json::iterator somebody = nlohmann::ext::find(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() == 20;
   });

   auto expectation = R"({
      "age": 20,
      "favorite_food": "pizza",
      "name": "Jason"
   })"_json;

   assert(expectation == *somebody);

   std::cout << std::endl << "find test passed.";
}

void unitTest_containsAndExcludes() {
    json people;
    addPeople(people);

    assert(nlohmann::ext::contains(people, [](json::iterator person) -> bool {
        return person->at("age").get<int>() == 20;
    }));

    assert(!nlohmann::ext::contains(people, [](json::iterator person) -> bool {
        return person->at("age").get<int>() > 49;
    }));

    assert(nlohmann::ext::excludes(people, [](json::iterator person) -> bool {
        return person->at("age").get<int>() < 19;
    }));

    std::cout << std::endl << "contains/excludes test passed.";
}

void unitTest_someNone() {
   json people;

   addPeople(people);

   assert(nlohmann::ext::some(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() == 20;
   }));

   assert(nlohmann::ext::none(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() == 200;
   }));

   std::cout << std::endl << "some/none test passed.";
}

void unitTest_findAll() {
   json people;

   addPeople(people, 20);

   auto oldGuys = nlohmann::ext::findAll(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() > 27;
   });

   assert(oldGuys.back()->at("age").get<int>() == 38);

   std::cout << std::endl << "findAll test passed.";
}

void unitTest_partition() {
   json people;

   addPeople(people, 6);

   auto part = nlohmann::ext::partition(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() <= 21;
   });

   auto pos = part.buildPositiveJson();

   // std::cout << std::endl << "partition positive: ";
   // std::cout << std::endl << pos.dump(3);
   // std::cout << std::endl << "partition negative: ";
   // std::cout << std::endl << part.buildNegativeJson().dump(3);

   assert(pos.size() == 3);

   // assert(part.negative.size() == 3);

   // std::cout << "Gaah: " << part.dumpPositive();
   // std::cout << "Gaah: " << part.dumpNegative();

   std::cout << std::endl << "partition test passed.";
}

void unitTest_intersection() {
   json A;
   json B;
   addPeople(A, 5);
   addPeople(B, 8);

   auto dump = nlohmann::ext::intersection(A, B);

   // for(auto& elem : dump) std::cout << std::endl << elem->dump(3);

   assert(dump.size() == 5);

   std::cout << std::endl << "unitTest_intersection passed.";
}

void unitTest_filter() {
   json people;

   addPeople(people);

   nlohmann::ext::filter(people, [people](json::iterator person) -> bool {
      return person->at("age").get<int>() != 20;
   });

   assert( 2 == people.size());
   assert(19 == people.front().at("age"));
   assert(21 == people.back() .at("age"));

   std::cout << std::endl << "filter test passed.";
}

void unitTest_iteratorCheck() {
   json people;

   addPeople(people);

   nlohmann::ext::filter(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() != 20;
   });

   json::iterator somebody = nlohmann::ext::find(people, [](json::iterator person) -> bool {
      return person->at("age").get<int>() == 21;
   });

   bool assertionMade = false;
   for(json::iterator itr = people.begin(); itr!= people.end(); ++itr) {
      if(somebody->at("age").get<int>() == itr->at("age").get<int>()) {
         assert(somebody == itr);
         assertionMade = true;
      }
   }

   assert(assertionMade);

   std::cout <<  std::endl << "iterator check test passed.";
}

void unitTest_append() {
   json arr;
   json appendage;
   addPeople(arr);
   addPeople(appendage, 5);

   nlohmann::ext::append(arr, appendage);

   assert(8 == arr.size());

   std::cout << std::endl << arr.dump(3);
}


int main() {
   unitTest_find();
   unitTest_containsAndExcludes();
   unitTest_someNone();
   unitTest_findAll();
   unitTest_partition();
   unitTest_intersection();
   unitTest_filter();
   unitTest_iteratorCheck();
   unitTest_append();
   return 0;
}
