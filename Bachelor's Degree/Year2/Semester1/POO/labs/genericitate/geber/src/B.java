abstract class A {
    static int x = 10;
    abstract void set(Integer value);
}

public class B {
    private static Integer num = 20;

    public static void main(String args[]) {
        A a = new A() {
            public void set(Integer value) {
                B.num = 30;
                x = value;
            }
        };
        a.set(B.num);
        A.x = 500;
        
        System.out.println(A.x);
    }
}