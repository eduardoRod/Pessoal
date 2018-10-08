def say_hi(name: str, age: int) -> str:
    return "Hi. My name is " + name + " and I'm " + str(age) + " years old"


def correct_sentence(text: str) -> str:
    if not text[0].isupper():
        text = text[0].upper() + text[1:]

    if not text.endswith("."):
        text += "."

    return text


def first_word(text: str) -> str:
    text = text.replace(",", " ")
    text = text.replace(".", " ")
    text = text.split()[0]
    return text


def second_index(text: str, symbol: str) -> [int, None]:
    if text.find(symbol, text.find(symbol)+1) == -1:
        return None

    return text.find(symbol, text.find(symbol)+1)


def between_markers(text: str, begin: str, end: str) -> str:

    ret = text
    if begin != "":
        if text.find(begin) != -1:
            ret = text.split(begin)[1]

    if end != "":
        if text.find(end) != -1:
            if ret.find(end) != -1:
                ret = ret.split(end)[0]
            else:
                ret = ""

    return ret


def best_stock(data):

    return list(data.keys())[list(data.values()).index(max(data.values()))]


name = "Eduardo"
age = 29
test_string = "olá Deusas do Olimpo"
symbol = "o"
marker_1 = "olá "
marker_2 = ""
stock = {'Olympus': 10.5,
         'Nokia': 190.8,
         'Samsung': 80.1}

print("say_hi: \n\tName: {} \n\tAge: {}\n".format(name, age))
print("\tResult: {}\n".format(say_hi(name, age)))

print("correct_sentence: \n\tText: {}".format(test_string))
print("\tResult: {}\n".format(correct_sentence(test_string)))

print("first_word: \n\tText: {}".format(test_string))
print("\tResult: {}\n".format(first_word(test_string)))

print("second_index: \n\tText: {}\n\tSymbol: {}".format(test_string, symbol))
print("\tResult: {}\n".format(second_index(test_string, symbol)))

print("between_markers: \n\tText: {}\n\tMarker 1: {}\n\tMarker 2: {}".format(test_string, marker_1, marker_2))
print("\tResult: {}\n".format(between_markers(test_string, marker_1, marker_2)))

print("best_stock: \n\tData: {}".format(stock))
print("\tResult: {}\n".format(best_stock(stock)))
