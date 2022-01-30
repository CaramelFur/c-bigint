int main()
{
  int a = -44;
  int b = 55;

  int* aa = &a;
  int* bb = &b;

  int c = *aa + *bb;

  return c;
}