#include <utility>
#include <variant>
#include <list>
#include <cstring>
#include "LindaTupleUtils.h"

std::string joinTupleElements(const Tuple &tuple, const std::string &joinStr) {
    return std::accumulate(
            tuple.begin() + 1,
            tuple.end(),
            formatTupleElement(*tuple.cbegin()),
            [joinStr](const TupleElement &a, const TupleElement &b) {
                return formatTupleElement(a) + joinStr + formatTupleElement(b);
            });
}

std::string formatTuple(const Tuple &tuple) {
    if (tuple.empty()) return "[ ]";
    const std::string &joinedElements = joinTupleElements(tuple, ", ");
    std::stringstream stream;
    stream << "[" << joinedElements << "]";
    return stream.str();
}

std::string formatTupleElement(const TupleElement &element) {
    std::stringstream stream;
    std::visit([&stream](auto &elem) { stream << elem; }, element);
    return stream.str();
}

enum TypeOfElement {
    intType, floatType, stringType
};

struct Element{
    std::string type;
    std::string oper;
    std::string cond;

    Element(std::string t, std::string o, std::string c) : type(std::move(t)), oper(std::move(o)), cond(std::move(c)) {}
};

class TuplePattern{
public:
    std::list<Element> tuplePatternElements;
};

TuplePattern stringToTuplePattern(const std::string &pattern) {
    TuplePattern tuplePattern;
    int i = 0;

    while(i < pattern.size()) {
        std::string type;
        while(pattern[i] != ':' && i < pattern.size()) {
            type.push_back(pattern[i]);
            ++i;
        }

        if(type != "integer" && type != "string" && type != "float") {
            std::cout << "Error in a tuple pattern" << std::endl;
        }

        std::string oper;
        while((pattern[i] == ':' || pattern[i] == '=' || pattern[i] == '<' || pattern[i] == '>') && i < pattern.size()) {
            oper.push_back(pattern[i]);
            ++i;
        }

        std::string cond;
        if(pattern[i] == '"') {
            ++i;
        }
        while(pattern[i] != ',' && i < pattern.size()) {
            cond.push_back(pattern[i]);
            ++i;
        }
        if(cond[cond.size() - 1] == '"') {
            cond = cond.substr(0, cond.size() - 1);
        }

        i += 2;

        Element element(type, oper, cond);
        tuplePattern.tuplePatternElements.push_back(element);

        std::cout << "Type: " << type << " Operator: " << oper << " Condition: " << cond << std::endl;
    }

    return tuplePattern;
}

bool checkIfMatches(const Tuple &tuple, const std::string &pattern) {

    TuplePattern tp = stringToTuplePattern(pattern);

    std::cout << tuple.size() << " " << tp.tuplePatternElements.size() << std::endl;
    if(tuple.size() != tp.tuplePatternElements.size()) {
        return false;
    }

    std::list<Element>::const_iterator iterator = tp.tuplePatternElements.begin();
    int firstNumber, secondNumber;

    for(std::variant<int, std::basic_string<char>, float> element : tuple) {
        TypeOfElement typeOfElement;

        try{
            typeOfElement = intType;
            std::cout << "Int: " << std::get<int>(element) << std::endl;
        }
        catch (const std::bad_variant_access&) {
            try{
                typeOfElement = floatType;
                std::cout << "Float: " << std::get<float>(element) << std::endl;
            }
            catch (const std::bad_variant_access&) {
                typeOfElement = stringType;
                std::cout << "String: " << std::get<std::string>(element) << std::endl;
            }
        }

        std::cout << "Type of pattern: " << (*iterator).type << std::endl;

        if(typeOfElement == intType && (*iterator).type == "integer") {
            std::cout << "Oper: " << (*iterator).oper << std::endl;

            if((*iterator).oper == ":") {
                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<int>(element);
                secondNumber = std::stoi((*iterator).cond);

                if(firstNumber == secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":<") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<int>(element);
                secondNumber = std::stoi((*iterator).cond);

                if(firstNumber < secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":<=")
            {
                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<int>(element);
                secondNumber = std::stoi((*iterator).cond);

                if(firstNumber <= secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":>") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*")
                {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<int>(element);
                secondNumber = std::stoi((*iterator).cond);

                if(firstNumber > secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":>=") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*")

                {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<int>(element);
                secondNumber = std::stoi((*iterator).cond);

                if(firstNumber >= secondNumber) {
                    ++iterator;
                    continue;
                }

            }

            return true;
        }

        else if(typeOfElement == floatType && (*iterator).type == "float") {

            std::cout << "Oper: " << (*iterator).oper << std::endl;

            if((*iterator).oper == ":") {
                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<float>(element);
                secondNumber = std::stoi((*iterator).cond);

                if(firstNumber == secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":<") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;
                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<float>(element);
                secondNumber = std::stof((*iterator).cond);

                if(firstNumber < secondNumber) {
                    ++iterator;
                    continue;
                }

            }

            else if((*iterator).oper == ":<=") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<float>(element);
                secondNumber = std::stof((*iterator).cond);

                if(firstNumber <= secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":>") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<float>(element);
                secondNumber = std::stof((*iterator).cond);

                if(firstNumber > secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            else if((*iterator).oper == ":>=") {

                std::cout << "Cond: " << (*iterator).cond << std::endl;

                if((*iterator).cond == "*") {
                    ++iterator;
                    continue;
                }

                firstNumber = std::get<float>(element);
                secondNumber = std::stof((*iterator).cond);

                if(firstNumber >= secondNumber) {
                    ++iterator;
                    continue;
                }
            }

            return false;
        }

        else if(typeOfElement == stringType && (*iterator).type == "string") {

            std::string operat = (*iterator).oper;

            const char * tupleString = std::get<std::string>(element).c_str();
            const char * patternString = (*iterator).cond.c_str();

            if (strcmp(patternString,"*") == 0) {}
            else if (operat == ":") {
                if (strcmp(tupleString, patternString) != 0)
                {
                    return false;
                }
            }

            else if (operat == ":<") {
                if (strcmp(tupleString, patternString) >= 0)
                {
                    return false;
                }
            }

            else if (operat == ":>") {
                if (strcmp(tupleString, patternString) <= 0)
                {
                    return false;
                }
            }

            else if (operat == ":<=") {
                if (strcmp(tupleString, patternString) > 0) {
                    return false;
                }
            }

            else if (operat == ":>=") {
                if (strcmp(tupleString, patternString) < 0)
                {
                    return false;
                }
            }

            ++iterator;
        }

        else {
            return false;
        }
    }

    return true;
}
