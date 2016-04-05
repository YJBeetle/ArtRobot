#ifndef JSON_H
#define JSON_H


class json
{
private:
    int8_t inited;
    JsonParser *parser;
    JsonNode *node;
    JsonReader *reader;

public:
    json();

    int8_t init(const char *jsondata);

    int32_t count();

    int8_t read_member(const char *member);
    int8_t end_member();
    int8_t read_element(int32_t i);
    int8_t end_element();

    const char * get_string(const char *item);
    int64_t get_int(const char *item);
    double get_double(const char *item);
};

#endif // JSON_H
