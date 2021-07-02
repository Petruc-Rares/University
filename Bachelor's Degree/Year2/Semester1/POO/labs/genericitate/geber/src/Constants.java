//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

public enum Constants {
    MY_VECTOR3,
    MY_MATRIX;

    private Constants() {
    }

    public static Constants stringToConstant(String classType) {
        byte var2 = -1;
        switch(classType.hashCode()) {
            case 1931923260:
                if (classType.equals("class MyVector3")) {
                    var2 = 0;
                }
                break;
            case -752721323:
                if (classType.equals("class MyMatrix")) {
                    var2 = 1;
                }
        }

        Constants var10000;
        switch(var2) {
            case 0:
                var10000 = MY_VECTOR3;
                break;
            case 1:
                var10000 = MY_MATRIX;
                break;
            default:
                var10000 = null;
        }

        return var10000;
    }
}
