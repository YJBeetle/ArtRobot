#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <json-glib/json-glib.h>

#include "json.h"

Json::Json()
{
    this->inited=0;
}

int8_t Json::init(const char *jsondata)
{
    if(this->inited)return 1;
    this->inited=1;

    parser=json_parser_new();
    json_parser_load_from_data(parser,jsondata,-1,NULL);
    node=json_parser_get_root(parser); //得到root结点
    reader=json_reader_new(node); //使用JsonReader来做解析
    return 0;
}

int32_t Json::count()
{
    if(!this->inited)return -1;
    return json_reader_count_elements(reader);
}

int8_t Json::read_member(const char *member)
{
    if(!this->inited)return 1;
    json_reader_read_member(reader,member);
    return 0;
}

int8_t Json::end_member()
{
    if(!this->inited)return 1;
    json_reader_end_member(reader);
    return 0;
}

int8_t Json::read_element(int32_t i)
{
    if(!this->inited)return 1;
    json_reader_read_element(reader,i); //读取第i个元素
    return 0;
}

int8_t Json::end_element()
{
    if(!this->inited)return 1;
    json_reader_end_element(reader); //返回上一个节点
    return 0;
}

const char * Json::get_string(const char *item)
{
    if(!this->inited)return 0;
    read_member(item); //得到该元素中的成员
    const char *value=json_reader_get_string_value(reader);
    end_member(); //返回上一个节点
    return value;
}

int64_t Json::get_int(const char *item)
{
    if(!this->inited)return 0;
    read_member(item); //得到该元素中的成员
    int64_t value=json_reader_get_int_value(reader);
    end_member(); //返回上一个节点
    return value;
}

double Json::get_double(const char *item)
{
    if(!this->inited)return 0;
    read_member(item); //得到该元素中的成员
    int64_t value=json_reader_get_double_value(reader);
    end_member(); //返回上一个节点
    return value;
}