#ifndef CLASS_H_
#define CLASS_H_
#include <stdlib.h>
/* Macros whose names end with double underscore are internal macros and not
 * ment for use outside the implementation.
 *
 * Use only macros under the "Public Macros" section, that doesn't end with a
 * trailing double underscore.
 *
 * All macros used should be encased within:
 
 * #define DEFINING_CLASS [classname]
 * //macro usage here
 * #undef DEFINING_CLASS
 */

/*Internal Macros*/

#define class_mutex__ &this->private->mutex
#define class_attribute__ &this->private->mutexAttr

#define class_conc__(x, y) x##y
#define class_conc2__(x, y) class_conc__(x, y)
#define class_if___1(t,f) t
#define class_if___0(t,f) f

#define class_if__(c, t, f) class_conc2__(class_if___, c)(t,f)
#define class_when__(c, t) class_if__(c, t, )

#define class_if___(t,f) t
#define class_if___CLASS_NO_PRIVATE(t,f) f
#define class_if_class_has_private__(body) class_if__(CLASS_NO_PRIVATE, , body)

#define class_arg16__(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define class_has_comma__(...) class_arg16__(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define class_trigger_parenthesis__(...) ,
 
#define class_arg_isempty__(...)  \
class_arg_isempty___(                                                   \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          class_has_comma__(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          class_has_comma__(class_trigger_parenthesis__ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          class_has_comma__(__VA_ARGS__ (/*empty*/)),                           \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          class_has_comma__(class_trigger_parenthesis__ __VA_ARGS__ (/*empty*/))      \
          )
 
#define class_paste5__(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define class_arg_isempty___(_0, _1, _2, _3) class_has_comma__(class_paste5__(class_is_empty_case_, _0, _1, _2, _3))
#define class_is_empty_case_0001_ ,

#define class_this__ struct DEFINING_CLASS *this
#define class_private__ struct class_conc2__(DEFINING_CLASS, _private)
#define class_func__(name) class_conc2__(DEFINING_CLASS, _##name)
#define class_private class_private__

#define class_begin__ {
#define class_end__ }

/*Public Macros*/

/*Mutex Attributes*/
#define class_lock_attribute pthread_mutexattr_t mutexAttr
#define class_lock_attribute_init(flags)                                        \
    do{                                                                         \
        pthread_mutexattr_init(class_attribute__);                              \
        pthread_mutexattr_settype(class_attribute__, flags);                    \
    }while(0)
#define class_lock_attribute_destroy()                                          \
    pthread_mutexattr_destroy(class_attribute__)

/*Class Mutex and locking*/
#define class_lock_object pthread_mutex_t mutex
#define class_lock_init(hasAttribute) pthread_mutex_init(class_mutex__,         \
                        class_if__(hasAttribute, class_attribute__, NULL))

#define class_lock_destroy() pthread_mutex_destroy(class_mutex__)

#define class_lock() pthread_mutex_lock(class_mutex__)
#define class_unlock() pthread_mutex_unlock(class_mutex__)

/*Class method/function declaration and definition templates*/
/*Class*/
#define defclass(class)                                                         \
    class_if_class_has_private__(class_private__;)                              \
    typedef struct class{
        
#define endclass                                                                \
        void (*destroy)(void *ptr);                                             \
        class_if_class_has_private__(class_private__ *private;)			\
    }DEFINING_CLASS;


#define classmethod(ret, name, ...)                                             \
    ret (*name)(const class_this__, ##__VA_ARGS__)

#define classfunction(ret, name, ...)                                           \
    ret (*name)(__VA_ARGS__)

#define classgetter(ret, name)							\
	classmethod(ret, class_conc2__(get,name))
#define classsetter(name, type)							\
	classmethod(void, class_conc2__(set,name), type name)
#define classproperty(name, type)						\
	classgetter(type, name);						\
	classsetter(name, type)

#define defclass_private                                                        \
    class_private__{

#define endclass_private };

/*End Class*/

/*CTOR and DTOR*/
        
#define decconstructor_named(class, name, ...)                                  \
    struct class *class_func__(name)(__VA_ARGS__)             

#define decconstructor(class, ...) decconstructor_named(class, new, ##__VA_ARGS__)

#define defconstructor_named(name, ...)                                         \
    struct DEFINING_CLASS *class_func__(name)(__VA_ARGS__)                      \
    class_begin__                                                               \
        class_this__ = malloc(sizeof(struct DEFINING_CLASS));    \
        class_if_class_has_private__(                                           \
            class_private__ *private = malloc(sizeof(class_private__));         \
            this->private = private;                                            \
        )                                                                       \
        this->destroy = class_func__(destroy);

#define defconstructor(...) defconstructor_named(new, ##__VA_ARGS__)

#define endconstructor                                                          \
        return this;                                                            \
    class_end__

#define decdestructor(class)                                                    \
    void class##_destroy(void *ptr)

#define defdestructor                                                           \
    void class_func__(destroy)(void *ptr)                                       \
    class_begin__                                                               \
        if(ptr == NULL) return;                                                 \
        class_this__ = ptr;                                                     \
        class_if_class_has_private__(                                           \
            class_private__ *private = this->private;                           \
        )

#define enddestructor                                                           \
        class_if_class_has_private__(free(private);)                            \
        free(this);                                                             \
    class_end__
/*End CTOR and DTOR*/

/*Methods*/
#define decmethod(ret, name, ...)                                               \
    static ret class_func__(name)                                               \
        (const class_this__, ##__VA_ARGS__)

#define defmethod(ret, name, ...)                                               \
    static ret class_func__(name)(const class_this__, ##__VA_ARGS__)\
        class_begin__                                                           \
        class_if_class_has_private__(                                           \
            class_private__ __attribute__((unused)) *private = this->private;   \
        )

#define endmethod                                                               \
    class_end__

/*End Methods*/

/*Properties*/
#define decgetter(ret, name)							\
	decmethod(ret, class_conc2__(get,name))
#define decsetter(name, type)							\
	decmethod(void, class_conc2__(set,name), type name)
#define decproperty(name, type)							\
	decgetter(type, name);							\
	decsetter(name, type)

#define defgetter(ret, name, customBody)					\
	defmethod(type, class_conc2__(get,name))				\
	class_if__(class_arg_ifempty__(customBody),				\
		return private->name;,						\
		customBody)
#define defsetter(name, type, customBody)					\
	defmethod(type, class_conc2__(set,name), type name) 			\
	class_if__(class_arg_ifempty__(customBody),				\
		private->name = name;,						\
		customBody)
		
#define defproperty(type, name, getCustomBody, setCustomBody)			\
	defgetter(type, name, getCustomBody)					\
	endgetter								\
	defsetter(void, name, setCustomBody)

#define endsetter }
#define endgetter }
#define endproperty }
/*End Properties*/

/*Functions*/
#define decfunction(ret, name, ...)                                             \
    static ret class_func__(name)(__VA_ARGS__)

#define decpubfunction(ret, name, ...)                                          \
    ret class_func__(name)(__VA_ARGS__)

#define deffunction(ret, name, ...)                                             \
    static ret class_func__(name)(__VA_ARGS__) class_begin__

#define defpubfunction(ret, name, ...)                                          \
    ret class_func__(name)(__VA_ARGS__) class_begin__

#define endfunction                                                             \
    class_end__
/*End Functions*/
#endif /*CLASS_H_*/
