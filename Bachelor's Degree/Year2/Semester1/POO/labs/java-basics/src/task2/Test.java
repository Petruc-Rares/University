package task2;

import java.util.Arrays;

public class Test {
    public static void main(String[] args) {
        Course curs = new Course();

        Student student1 = new Student();
        student1.setName("Gigi");
        student1.setYear(2008);

        Student student2 = new Student();
        student2.setName("Cosmina");
        student2.setYear(2010);

        Student student3 = new Student();
        student3.setName("Alex");
        student3.setYear(2008);

        Student student4 = new Student();
        student4.setName("Mihaela");
        student4.setYear(2010);

        curs.setTitle("POO");
        curs.setDescription("un curs interesant");

        Student[] students = {student1, student2, student3, student4};
        curs.setStudents(students);

        Student[] goodStudents = curs.filterYear(Integer.parseInt(args[0]));
        System.out.println("Studentii de elita au fost " + Arrays.toString(goodStudents));
    }
}
