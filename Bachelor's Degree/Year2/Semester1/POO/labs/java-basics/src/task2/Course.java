package task2;

public class Course {
    private String title;
    private String description;
    private Student[] students;

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Student[] getStudents() {
        return students;
    }

    public void setStudents(Student[] students) {
        this.students = students;
    }

    Student[] filterYear(int year) {
        Student[] desiredStudents = new Student[4];
        int noStudents = 0;
        for (Student student : students) {
            if (student.getYear() == year) {
                desiredStudents[noStudents] = student;
                noStudents++;
            }
        }
        return desiredStudents;
    }


}
