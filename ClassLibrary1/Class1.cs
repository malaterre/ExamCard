using System;

namespace ClassLibrary1
{
    [Serializable]
    public class Class1
    {
        public Class1()
        {
            BB = new B();
        }

        public int a;
        public B BB;
    }
    [Serializable]
    public class B
    {
        public int A1 { get; set; }

        public int A2 { get; set; }
    }
}
