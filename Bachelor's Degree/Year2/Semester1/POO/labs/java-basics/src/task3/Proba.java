package task3;

import task2.Course;
import task2.Student;

import java.util.Arrays;

public class Proba {
    public static void main(String[] args) {
        Student student1 = new Student();
        student1.setName("Gigi");
        student1.setYear(2008);

        Student student2 = new Student();
        student2.setName("Gigi");
        student2.setYear(2008);

        System.out.println(student1.equals(student2));
    }
}
