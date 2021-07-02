package src.com.oop_pub.exceptions.ex1;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class MainEx1 {
    private static void readAndPrintLine() {
        BufferedReader reader = null;

        try {
            reader = new BufferedReader(new InputStreamReader(System.in));
            String str;
            System.out.print("Introduceti stringul:");
            str = reader.readLine();
            System.out.println(str);
            reader.close();

        } catch (IOException io) {
            System.out.println("S-a intamplat ceva gresit: " + io);
            io.printStackTrace();
        } finally {
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        readAndPrintLine();
    }
}
