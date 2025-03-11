## Objective-C++

Objective-C++ is a programming language that combines the features of both Objective-C and C++. It allows you to use C++ code within an Objective-C project and vice versa. This is particularly useful when you need to integrate C++ libraries or perform low-level operations in an Objective-C environment, such as in iOS development.

### 0. Key Features of Objective-C++
1. Combination of Objective-C and C++:
    Objective-C++ allows you to write code that mixes Objective-C and C++ syntax.
    You can use C++ classes, templates, and standard library features alongside Objective-C objects and messaging.
2. File Extension:
    Objective-C++ files have the .mm extension instead of .m (used for pure Objective-C files).
3. Interoperability:
    Objective-C++ supports seamless interaction between Objective-C objects and C++ objects.
    You can call C++ functions and methods from Objective-C code and vice versa.

### 1. interface & implementation

#### 1.1 @interface

You shall declare `@interface` for an interface class that wraps up C++ functionalities.

To make any `@interface` class effective, an `@implementation` part shall be added to the `.mm` files.

#### 1.2 @implementation
In the curly braces, variables are assigned, after which methods are implemented.

```objc
@implementation ClassName {
    // Instance variables (optional)
}
// Method implementations
@end
```

#### 1.3 method declaration & definition

Method declaration: (declared between @interface and @end)
```objc
#import <Foundation/Foundation.h>

@interface MyClass : NSObject

- (returnType)instanceMethodName:(parameterType)parameterName;
+ (returnType)classMethodName:(parameterType)parameterName;

@end

```

Method definition:
```objc
// single parameter
- (returnType)instanceMethodName:(parameterType)parameterName {
    // Instance Method implementation
}
+ (returnType)classMethodName:(parameterType)parameterName {
    // Class Method implementation
}
// multiple parameters
- (returnType)instanceMethodName:(parameterType1)parameterName1 parameter2Label:(parameterType2)parameterName2 {
    // Instance Method implementation
}
+ (returnType)classMethodName:(parameterType)parameterName parameter2Label:(parameterType2)parameterName2 {
    // Class Method implementation
}
```

#### 1.4 method calling
1. Objective-C++ calling in Objective-C++:
    ```objc
    [instance methodName:parameter1 parameter2Label:parameter2];
    ```

2. C++ functions are called with C++ style in Objective-C++. Both functions can be called within 1 Objective-C++ function.
    ```objc
    // for example

    NSError *error = nil;
    // objc style
    NSData *configData = [NSData dataWithContentsOfFile:[NSString stringWithUTF8String:config_path.c_str()]];
    NSDictionary *configDict = [NSJSONSerialization JSONObjectWithData:configData options:0 error:&error];
    // If use_mmap key doesn't exist, default to YES
    BOOL useMmap = configDict[@"use_mmap"] == nil ? YES : [configDict[@"use_mmap"] boolValue];

    // c++ style
    llm.reset(Llm::createLLM(config_path));
    if (!llm) {
        return NO;
    }
    ```

3. Objective-C++ calling in Swift:
    ```swift
    object.methodName(parameter1, parameter2Label: parameter2)
    ```

    Bridging is necessary to bridge between Objective-C++ and Swift.
    - The bridging header is necessary if you are working in a Swift project that includes Objective-C code. It allows Swift to see the Objective-C classes and methods.
    - Ensure the bridging header is correctly set up in your project settings under `Swift Compiler` - `General` -> `Objective-C Bridging Header`.



