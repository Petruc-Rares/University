//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//


import jdk.swing.interop.SwingInterOpUtils;

public class MyVector3 implements Sumabil {
    private int x;
    private int y;
    private int z;

    public MyVector3(int x, int y, int z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public void addValue(Sumabil value) {
        System.out.println(value.getClass().toString());
        if (Constants.stringToConstant(value.getClass().toString()) == Constants.MY_VECTOR3) {
            MyVector3 copyValue = (MyVector3)value;
            this.x += copyValue.getX();
            this.y += copyValue.getY();
            this.z += copyValue.getZ();
        } else {
            System.out.println("can't add value");
        }
    }

    public int getX() {
        return this.x;
    }

    public int getY() {
        return this.y;
    }

    public int getZ() {
        return this.z;
    }

    public String toString() {
        return "MyVector3{x=" + this.x + ", y=" + this.y + ", z=" + this.z + "}";
    }
}
