# json_ext 
extends nlohmann::json with a few methods that should be familiar to the avid javascript/typescript programmer.
All methods presume we are dealing with a json array of objects.
There are methods which return a `json::iterator` or `std::vector<json::iterator>`s (or similar): find, findAll, partition, intersection.
Other methods give you a yes-no answer on the json array. These methods are: some, none, contains, excludes
Lastly, the `filter()` method filters the json container in place (by erasing elements which fail the callback function).
The methods do not build new json objects but instead give you `std::vector<json::iterator>`s with a few execptions.
This means you can still compare the resulting `json::iterator` to iterators of the original json array.

diclaimer: I doubt this is good practice (though the code does about what I need it to do).

