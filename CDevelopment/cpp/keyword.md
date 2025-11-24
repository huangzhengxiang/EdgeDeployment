# keyword

## Function Programming

### Function Pointer
A function pointer stores the address of a function and allows you to call it indirectly.
Declaration of a function pointer:
```cpp
return_type (*ptr_name)(param_types);
// example
int add(int a, int b) { return a + b; }
int main() {
    int (*func)(int, int) = add;  // store function address
}
```


### Lambda Expression
A lambda expression creates an anonymous function directly in place, optionally capturing surrounding variables.
Declaration of a lambda expression:
```cpp
[capture](params) -> return_type { body }
```
This line gives us a function pointer. `-> return_type` can be omitted if return type is deducible to the compiler.

#### Parameter Capture Mode
1. [=] — Capture everything by value
    Copy value. Cannot modify inside lambda unless mutable is used.
2. [&] — Capture everything by reference
    Directly modify the original values. But dangerous if lambda function in a separate thread that may ends after its context.
3. [x]/[&x] — Explicit Single Captures
4. [=, &y] — Mixed Expression

You can capture specific variables by value:


## Class Oriented Programming (COP related)

### Class Derivation

#### [Derived]:[Base]
`:` is the delimiter to specify the inheritance relationship between Derived class and Base class.

#### virtual
https://en.cppreference.com/w/cpp/language/virtual
- **non-virtual function and *hide***: Non-virtual functions are resolved statically at Compile-time, While Virtual functions are resolved dynamically at Run-time. A function declared in derived class ***hides all functions of the same name*** in the base class compile-time, ineffective for a base class pointer storing a derived class object.

- ***virtual* function and *override***: Virtual functions are member functions whose behavior can be overridden in derived classes. As opposed to non-virtual functions, the overriding behavior is preserved even if there is no compile-time information about the actual type of the class. That is to say, if a derived class is handled using pointer or reference to the base class, a call to an overridden virtual function would invoke the behavior defined in the derived class. Such a function call is known as virtual function call or virtual call. A virtual funtion is a function ***with the same name, parameter-type-list, cv-qualification, and ref-qualifier (or absence of same)*** as the virtual function in the base class. Otherwise, it *hides* the base function.

- <b>`= 0`</b> A virtual function without implementation, leading to a virtual class, can't be instantiated.

#### final
https://www.geeksforgeeks.org/c-final-specifier/
(same in Java)
- function: Virtual functions that can't be overrided by derived classes.
- class: If a class or struct is marked as final then it becomes non inheritable and it cannot be used as base class/struct. 

#### `= delete`
https://stackoverflow.com/questions/5513881/meaning-of-delete-after-function-declaration
A deleted function is not declared and defined by default. Copy functions, constructor, implicit converted functions are usually deleted functions.

#### explicit
https://www.geeksforgeeks.org/use-of-explicit-keyword-in-cpp/
To specify that a constructor can't be called to form implicit data type conversion, leading to `can't convert xxx to xxx implicitly`.

#### `static` function 
https://www.geeksforgeeks.org/static-member-function-in-cpp/
`static` members don't belong to objects, but belongs to class.

#### const
- appending const member function: 
    function that doesn't change the value of members.

- prepending const member function:
    qualifying that the returned value is a constant.


### Class Template and Specialization

#### Declaration
```cpp
template<typename T> class A;
```

#### 3 Ways of Specialization
1. Primary template	
   ```cpp
   template<typename T> class A { ... };
   ```

2. Full specialization	
   The `template<>` prefix is mandatory, distinguishing the template class than a non-template one, avoiding redefinition error.
   ```cpp
   template<> class A<int> { ... };
   ```
   For an unsupported type, compile error incomplete type raises.

3. Partial specialization	
   In this case, the specialization is still a template, specializing the cases where the original template type being an std::vector. This specialization itself is also a template which takes a new `typename T`.
   ```cpp
   template<typename T> class A<std::vector<T>> { ... };
   ```