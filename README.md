# newt

newt is an exploration of type safety and immutability, aiming to combine the succinctness and expressiveness of languages such as Python and Ruby with the type safety of the C family of programming languages.

newt is derived from [a compilers class project](http://www.ecst.csuchico.edu/~tyson/classes/515.s15/), and is also a fulfillment of a project requirement for completion of a Bachelor's Degree of Science in Computer Science with Honors from CSU Chico.

newt is [free software](http://www.gnu.org/philosophy/free-sw.en.html), licensed under the [GPLv3](http://www.gnu.org/licenses/gpl-3.0.en.html).

# Philosophy
As creators of software, automation is what we do. We create software because we have the idea that this automation yields value. If we actually believe this idea that automation is valuable, we will apply it everywhere, including the software tools with which we create software (with some [exceptions](https://xkcd.com/1205/)).

Automation leads to tighter feedback loops, and newt built on the principle that tighter feedback loops yield better solutions, and deliver the solution faster. Here "feedback loop" encompasses everything necessary to create a solution and be confident that it works as intended, and includes authoring code, compilation (if necessary), and all forms of testing.

"boilerplate" code and the manually synchronized declaration and definition exemplified by header files are examples of things that lack automation, and newt's creators are inspired by the succinctness and expressiveness found in languages such as [Python](https://www.python.org/) to avoid such constructs. However, newt is statically typed: variables are associated with a type and may only be assigned of that type (or its subtypes). This is because dynamic typing has been observed to lead to subtle defects, and also requires a great deal of non-automated unit testing to achieve the same correctness guarantees that are more succinctly and precisely expressed in a static type system.

newt also aims to automate immutability, as immutable data structures have been observed to avoid or mitigate many classes of defects. Little prior art is known to exist for such language constructs, so new syntax is introduced to achieve this goal. 

newt is built with the understanding that non-astonishing behavior is important, and that "non-astonishing" frequently just means "familiar". Where possible, existing conventions and well-known programming idioms are preserved.

# Syntax

newt's syntax is a blend of C-style language constructs and notation from more succinct grammars. The grammar does not include semi-colon statement terminators. Whitespace is not significant; blocks are surrounded by curly braces.

## Hello, World!
```
print("Hello, World!")
```

## Comments
```
print("Hello, World!") #prints "Hello, World!"
```

## Variable Declaration

Variable declaration is a departure from the C-style syntax. Variables are declared by first specifying the variable name, followed by a colon, followed by the variable's type specifier. Variables may be initialized using an appropriate (that is, type-compatible) value. If the variable initializer is specified, the type specifier may be omitted, allowing the variable's type to be inferred.

```
a_lovely_var:int = 42
a_type_inferred_var:= "danny boy" #the type is inferred from the type of the initialization value
a_var:double #every type in newt has an associated default value.
```
### Builtin types
The following types are builtin:
* boolean (default value: false)
* int (default value: 0)
* double (default value: 0.0)
* string (default value: "")


## Variable Assignment
```
a_lovely_var:int = 42 #variables must be declared before assignment
a_lovely_var = 10
```
## Arrays
Arrays are dynamically sized and multiple dimensions are permitted.

```
a_lovely_array:int[]
a_lovely_multidimensional_array:string[][]

a_lovely_array[5] = 1
a_lovely_multidimensional_array[3][1] = "n"
```

## Default Values
The default value of any type can be accessed using the @ operator
```
a_lovely_var:int = @int 
```
The default value of a type is fixed and constant for the lifetime of a program's execution.

## Structs
```
struct Person {
	age:int
	name:string = "Jojo" #default member values allowed
}
```

Structs are instantiated from another instance of a struct, or the struct's default value, using the `with` operator:

```
	p1:Person = @Person with { age = 25, name = "Joe" }
	p2 = p1 with { age = 26 }
```

Structs may be declared readonly, in which case it is a semantic error to re-assign its member values:

```
readonly struct Point {
	x:int
	y:int
}

point:Point = @Point with { x = 30, y = 40 }
point.x = 50 #semantic error
```

## Functions

Functions are first-class citizens in newt, assignable to variables. The syntax for declaring functions is one of the most notable departures from C-style syntax:

```
a:= (a: int, b:int) -> int {
	return a + b
}
```

Higher order functions are also supported. That is, functions may be passed as function arguments:

```
f:= (a:int, b:(int) -> int) -> int {
	return b(a)
}

g:= (d:int) -> int {
	return d * d
}

x:= f(3, g)
y:= f(3, (e:int) -> int { return e * e * e }) #anonymous functions are supported too
```

Functions need not be declared with a body, as every type has a default value. The default value of a function type is a placeholder function that returns the default value of its return type.

```
f:(int) -> int  #will return the default value of the int type if invoked
```
