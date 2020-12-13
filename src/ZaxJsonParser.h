/***************************************************************************
* Copyright 2020 Tamas Levente Kis                                         *
*                                                                          *
* Licensed under the Apache License, Version 2.0 (the "License");          *
* you may not use this file except in compliance with the License.         *
* You may obtain a copy of the License at                                  *
*                                                                          *
*     http://www.apache.org/licenses/LICENSE-2.0                           *
*                                                                          *
* Unless required by applicable law or agreed to in writing, software      *
* distributed under the License is distributed on an "AS IS" BASIS,        *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
* See the License for the specific language governing permissions and      *
* limitations under the License.                                           *
***************************************************************************/

#ifndef _ZAXJSONPARSER_H_
#define _ZAXJSONPARSER_H_

struct ZaxStringWrap
{
    const char* m_str;
    ZaxStringWrap(const char* a_str);
    bool operator < (const ZaxStringWrap &a_rhs) const;
};

class ZaxJsonFlatParser
{
    char* m_own_buffer;
public:
    std::map<ZaxStringWrap, const char*> m_values;
    std::vector<const char*> m_list_values;
    ZaxJsonFlatParser(const char* a_json, bool a_in_situ = true, bool* a_success = 0);
    virtual ~ZaxJsonFlatParser();
};

class ZaxJsonParser
{
    static unsigned int initial_alloc_size_;
    static unsigned int maximum_alloc_size_;

    template <typename vtype>
    static bool print_val(char* a_json, const char* a_json_buffer_end, const vtype& a_val, bool a_add_comma = true)
    {
        bool result = true;
        unsigned int alloc_size = initial_alloc_size();
        char* json = new char[alloc_size];
        while (!a_val.to_json(json, json + alloc_size - 1))
        {
            delete[] json;
            alloc_size *= 2;
            if (alloc_size > maximum_alloc_size())
                return false;
            json = new char[alloc_size];
        }
        if (a_add_comma)
            result = (snprintf(a_json, a_json_buffer_end - a_json, "%s, ", json) < a_json_buffer_end - a_json);
        else
            result = (snprintf(a_json, a_json_buffer_end - a_json, "%s", json) < a_json_buffer_end - a_json);
        delete[] json;
        return result;
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const int a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "%d, ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "%d", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const unsigned int a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "%u, ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "%u", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const double a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "%lf, ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "%lf", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const long long a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "%I64u, ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "%I64u", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const char a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "%c, ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "%c", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const float a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "%f, ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "%f", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const char* a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "\"%s\", ", a_val) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "\"%s\"", a_val) < a_json_buffer_end - a_json);
    }

    static bool print_val(char* a_json, const char* a_json_buffer_end, const std::string& a_val, bool a_add_comma = true)
    {
        if (a_add_comma)
            return (snprintf(a_json, a_json_buffer_end - a_json, "\"%s\", ", a_val.c_str()) < a_json_buffer_end - a_json);
        else
            return (snprintf(a_json, a_json_buffer_end - a_json, "\"%s\"", a_val.c_str()) < a_json_buffer_end - a_json);
    }

    template <typename vet>
    static bool print_val(char* a_json, const char* a_json_buffer_end, const std::vector<vet>& a_vals, bool a_add_comma = true)
    {
        return ZaxJsonParser::append(a_json, a_json_buffer_end, "", a_vals, a_add_comma);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const char* a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":\"%s\", ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":\"%s\"", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const std::string& a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":\"%s\", ", a_key, a_val.c_str()) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":\"%s\"", a_key, a_val.c_str()) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const int a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%d, ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%d", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const unsigned int a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%d, ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%d", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const double a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%lf, ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%lf", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const long long a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%I64u, ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%I64u", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const char a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%c, ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%c", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const float a_val, bool a_add_comma = true)
    {
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%f, ", a_key, a_val) < a_json_buffer_end - _current_json);
        else
            return (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%f", a_key, a_val) < a_json_buffer_end - _current_json);
    }

    template <typename vtype>
    static bool print_key_and_val(char* a_json, const char* a_json_buffer_end, const char* a_key, const vtype& a_val, bool a_add_comma = true)
    {
        bool result = true;
        unsigned int alloc_size = initial_alloc_size();
        char* json = new char[alloc_size];
        while (!a_val.to_json(json, json + alloc_size - 1))
        {
            delete[] json;
            alloc_size *= 2;
            if (alloc_size > maximum_alloc_size())
                return false;
            json = new char[alloc_size];
        }
        char* _current_json = a_json + strlen(a_json);
        if (a_add_comma)
            result = (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%s, ", a_key, json) < a_json_buffer_end - _current_json);
        else
            result = (snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":%s", a_key, json) < a_json_buffer_end - _current_json);
        delete[] json;
        return result;
    }

    static void get_val(std::string& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = a_json;
    }

    static void get_val(char* a_dst, const char* a_json, std::string* a_error_output)
    {
        strcpy(a_dst, a_json);
    }

    static void get_val(int& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = atoi(a_json);
    }

    static void get_val(unsigned int& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = atoi(a_json);
    }

    static void get_val(double& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = atof(a_json);
    }

    static void get_val(long long& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = atoll(a_json);
    }

    static void get_val(char& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = atoi(a_json);
    }

    static void get_val(float& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst = atof(a_json);
    }

    template <typename vet>
    static void get_val(std::vector<vet>& a_dst, const char* a_json, std::string* a_error_output)
    {
        ZaxJsonParser::parse(a_dst, " unnamed list", a_json, a_error_output);
    }

    template <typename vtype>
    static void get_val(vtype& a_dst, const char* a_json, std::string* a_error_output)
    {
        a_dst.from_json(a_json);
    }

public:
    static unsigned int initial_alloc_size();
    static unsigned int maximum_alloc_size();
    static void set_initial_alloc_size(unsigned int a_size);
    static void set_maximum_alloc_size(unsigned int a_size);

    template <typename vt>
    static bool append(char* a_json, const char* a_json_buffer_end, const char* a_key, const vt& a_value, bool a_add_comma = true)
    {
        return print_key_and_val(a_json, a_json_buffer_end, a_key, a_value, a_add_comma);
    }

    template <typename vt>
    static bool append(char* a_json, const char* a_json_buffer_end, const char* a_key, const std::vector<vt>& a_values, bool a_add_comma = true)
    {
        bool _result = true;
        char* _current_json = a_json + strlen(a_json);
        if (a_key && strlen(a_key))
            _result = snprintf(_current_json, a_json_buffer_end - _current_json, "\"%s\":[", a_key) < a_json_buffer_end - _current_json;
        else
        {
            if (a_json_buffer_end - _current_json > 1)
                strcat(a_json, "[");
            else
                _result = false;
        }
        if (_result)
        {
            a_json += strlen(a_json);
            unsigned int nr_vals = a_values.size();
            if (nr_vals > 1)
                for (unsigned int i = 0; i < nr_vals - 1; ++i)
                {
                    if (!print_val(a_json, a_json_buffer_end, a_values[i]))
                    {
                        _result = false;
                        break;
                    }
                    a_json += strlen(a_json);
                }
            if (_result)
            {
                if (nr_vals)
                    _result = print_val(a_json, a_json_buffer_end, a_values[nr_vals - 1], false);
                if (_result && (a_json_buffer_end - _current_json > 3))
                {
                    if (a_add_comma)
                        strcat(a_json, "], ");
                    else
                        strcat(a_json, "]");
                }
                else
                    _result = false;
            }
        }
        return _result;
    }

    template <typename type>
    static void parse(type& a_dst, const char* a_property, const char* a_json, std::string* a_error_output)
    {
        if (a_json)
            get_val(a_dst, a_json, a_error_output);
        else if (a_error_output)
            (*a_error_output) += std::string("WARNING: JSON property is missing: '") + a_property + "'\n";
    }

    template <typename vt>
    static void parse(std::vector<vt>& a_vect, const char* a_property, const char* a_json, std::string* a_error_output)
    {
        if (a_json)
        {
            bool success = false;
            ZaxJsonFlatParser vector_data(a_json, true, &success);
            if (!success)
                (*a_error_output) += std::string("ERROR: error parsing JSON: '") + a_json + "'\n";
            if (vector_data.m_list_values.size())
            {
                a_vect.resize(vector_data.m_list_values.size());
                for (unsigned int i = 0; i < vector_data.m_list_values.size(); ++i)
                    get_val(a_vect[i], vector_data.m_list_values[i], a_error_output);
            }
        }
        else if (a_error_output)
            (*a_error_output) += std::string("WARNING: JSON property is missing: '") + a_property + "'\n";
    }
};

template <typename mt>
std::pair<std::string, mt*> json_property(std::string a_name, mt& a_obj)
{
    return std::make_pair(a_name, &a_obj);
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
zax_to_json(char* a_json, const char* a_json_buffer_end, bool& a_result, std::tuple<Tp...> a_tuple, bool a_insert_object_trails = true)
{}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if < I < sizeof...(Tp), void>::type
zax_to_json(char* a_json, const char* a_json_buffer_end, bool& a_result, std::tuple<Tp...> a_tuple, bool a_insert_object_trails = true)
{
    if (a_result)
    {
        if (!I)
        {
            if (a_json_buffer_end - a_json > 1)
            {
                if (a_insert_object_trails)
                    strcpy(a_json, "{");
                else
                    a_json[0] = 0;
            }
            else
                a_result = false;
        }
        if (!a_result)
            return;
        if (I < sizeof...(Tp) - 1)
            a_result = ZaxJsonParser::append(a_json, a_json_buffer_end, std::get<I>(a_tuple).first.c_str(), *std::get<I>(a_tuple).second);
        else
            a_result = ZaxJsonParser::append(a_json, a_json_buffer_end, std::get<I>(a_tuple).first.c_str(), *std::get<I>(a_tuple).second, false);
        if (!a_result)
            return;
        zax_to_json <I + 1, Tp...> (a_json, a_json_buffer_end, a_result, a_tuple, a_insert_object_trails);
        if (!a_result)
            return;
        if ((I == sizeof...(Tp) - 1) && a_insert_object_trails)
        {
            if ((a_json_buffer_end - (a_json + strlen(a_json))) > 1)
                strcat(a_json, "}");
            else
                a_result = false;
        }
    }
}

#define JSON_PROPERTY_(member) json_property(#member, object_to_convert.member)
#define JSON_PROPERTY_NAME(member, name_in_json) json_property(name_in_json, object_to_convert.member)
#define JSON_PROPERTY_CHOOSER(no_arg,member,name_in_json,FUNC, ...) FUNC
#define JSON_PROPERTY(...) JSON_PROPERTY_CHOOSER(no_arg,##__VA_ARGS__, JSON_PROPERTY_NAME(__VA_ARGS__), JSON_PROPERTY_(__VA_ARGS__))

#define zax_convert_to_json(arg_json, a_json_buffer_length, arg_obj, arg_json_properties, ...) ({\
    bool _result = true;\
    decltype(arg_obj)& object_to_convert = arg_obj; \
    const char* _json_buffer_end = arg_json + a_json_buffer_length; \
    zax_to_json(arg_json, _json_buffer_end, _result, std::make_tuple(arg_json_properties), ##__VA_ARGS__);\
    _result;\
})

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
zax_from_json(const char* a_json, std::tuple<Tp...> a_tuple, ZaxJsonFlatParser* parsed_json, std::string* a_error_output = 0)
{}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if < I < sizeof...(Tp), void>::type
zax_from_json(const char* a_json, std::tuple<Tp...> a_tuple, ZaxJsonFlatParser* parsed_json, std::string* a_error_output = 0)
{
    if (!parsed_json)
    {
        bool success = false;
        parsed_json = new ZaxJsonFlatParser(a_json, false, &success);
        if (!success)
            (*a_error_output) += std::string("ERROR: error parsing JSON: '") + a_json + "'\n";
    }
    if (I < sizeof...(Tp))
        ZaxJsonParser::parse(*std::get<I>(a_tuple).second, std::get<I>(a_tuple).first.c_str(), parsed_json->m_values[std::get<I>(a_tuple).first.c_str()], a_error_output);
    zax_from_json <I + 1, Tp...> (a_json, a_tuple, parsed_json, a_error_output);
    if (I == sizeof...(Tp) - 1)
        delete parsed_json;
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
zax_from_json(char* a_json, std::tuple<Tp...> a_tuple, ZaxJsonFlatParser* parsed_json, std::string* a_error_output = 0)
{}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if < I < sizeof...(Tp), void>::type
zax_from_json(char* a_json, std::tuple<Tp...> a_tuple, ZaxJsonFlatParser* parsed_json, std::string* a_error_output = 0)
{
    if (!parsed_json)
    {
        bool success = false;
        parsed_json = new ZaxJsonFlatParser(a_json, true, &success);
        if (!success)
            (*a_error_output) += std::string("ERROR: error parsing JSON: '") + a_json + "'\n";
    }
    if (I < sizeof...(Tp))
        ZaxJsonParser::parse(*std::get<I>(a_tuple).second, std::get<I>(a_tuple).first.c_str(), parsed_json->m_values[std::get<I>(a_tuple).first.c_str()], a_error_output);
    zax_from_json <I + 1, Tp...> (a_json, a_tuple, parsed_json, a_error_output);
    if (I == sizeof...(Tp) - 1)
        delete parsed_json;
}

#define zax_convert_from_json(arg_json, arg_obj, arg_json_properties, ...) decltype(arg_obj)& object_to_convert = arg_obj; zax_from_json(arg_json, std::make_tuple(arg_json_properties), 0, ##__VA_ARGS__);

#define ZAX_JSON_SERIALIZABLE_WDC(class_name, ...)\
    virtual void from_json(const char* a_json)\
    {\
        zax_convert_from_json(a_json, *this, ##__VA_ARGS__);\
    }\
    virtual bool to_json(char* a_json, const char* a_json_buffer_end) const\
    {\
        return zax_convert_to_json(a_json, (a_json_buffer_end - a_json), *this, ##__VA_ARGS__);\
    }\
    class_name(const char* a_json)\
    {\
        *this = a_json;\
    }\
    class_name(const string& a_json)\
    {\
        *this = a_json;\
    }\
    void operator = (const char* a_json)\
    {\
        zax_convert_from_json(a_json, *this, ##__VA_ARGS__);\
    }\
    void operator = (const string& a_json)\
    {\
        zax_convert_from_json(a_json.c_str(), *this, ##__VA_ARGS__);\
    }\
    template <typename T> operator T() const\
    {\
        unsigned int alloc_size = ZaxJsonParser::initial_alloc_size();\
        char* json = new char[alloc_size];\
        while (!zax_convert_to_json(json, alloc_size - 1, *this, ##__VA_ARGS__))\
        {\
            delete[] json;\
            alloc_size *= 2;\
            json = new char[alloc_size];\
            if (alloc_size > ZaxJsonParser::maximum_alloc_size())\
                break;\
        }\
        string result(json);\
        delete[] json;\
        return result;\
    }\
    friend std::ostream& operator<<(std::ostream& os, const class_name& a_obj)\
    {\
        string s = a_obj;\
        return os << s;\
    }\

#define ZAX_JSON_SERIALIZABLE(class_name, ...)\
    class_name() {}\
    ZAX_JSON_SERIALIZABLE_WDC(class_name, ##__VA_ARGS__)\

#endif /// _ZAXJSONPARSER_H_
