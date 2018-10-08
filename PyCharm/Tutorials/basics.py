
def func(v = 2):
    print(v)
    print(id(v))
    v = 213
    return v

array = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
print(array)
print(type(array))
print(type(1))
print(type([1, 2]))
print(type(2.0))
dictionary = {"1+1=":2, "2+2=":4}
print(type(dictionary))
print("1+1=", dictionary["1+1="])



array.append(1.1)
print(array)

sum = 0
for x in array:
    sum += x
    print(x)

print(sum)


print(id(sum))

func()

print(sum)

sum = "hi"
print(sum)
