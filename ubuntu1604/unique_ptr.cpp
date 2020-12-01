#include <memory>
#include <cstdio>
using namespace std;

struct A
{
 A()
 {
  puts("A");
 }
 ~A()
 {
  puts("~A");
 }
};

void f(A*)
{
 puts("f");
}

int main(int argc, char const *argv[])
{
 f(make_unique<A>().get());
 unique_ptr<A> p = make_unique<A>();
 f(p.get());
 auto p2 = make_unique<A>().get();
 f(p2);
}