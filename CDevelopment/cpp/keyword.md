## keyword

### Class Oriented Programming (COP related)

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

