# zax-parser
Zax is a very basic JSON parser implemented in C++11 with probably the simplest possible interface, providing a kind of a struct to JSON and JSON to struct conversion.

### Code:

```cpp

struct some_class
{
    int x = 9;
    string title = "some title";
    std::vector<std::vector<int>> scores = {{1, 2, 3}, {4, 5, 6}};

    ZAX_JSON_SERIALIZABLE(some_class, (JSON_PROPERTY(x), JSON_PROPERTY(title), JSON_PROPERTY(scores)))
};

some_class some_obj = "{\"x\":17, \"title\":\"test text\", \"scores\":[[1, 1, 2], [3], [5, 8], [13, 21]]}";

string some_json = some_obj;

cout << some_json << endl;

```
### Result:

```cpp

{"x":17, "title":"test text", "scores":[[1, 1, 2], [3], [5, 8], [13, 21]]}

```

[Please check the documentation](https://tamask1s.github.io/zax-parser/index.html)
