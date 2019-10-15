#ifndef JF_UTIL_UITL_H
#define JF_UTIL_UITL_H

#define INSTANCE_IMP(class_name, ...) \
class_name &class_name::Instance() { \
    static std::shared_ptr<class_name> s_instance(new class_name(__VA_ARGS__)); \
    static class_name &s_insteanc_ref = *s_instance; \
    return s_insteanc_ref; \
}

#endif  // JF_UTIL_UITL_H
