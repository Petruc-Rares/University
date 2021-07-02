package com.poo.lab12.reports;

import java.util.*;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import com.poo.lab12.entities.Account;
import com.poo.lab12.entities.Bank;
import com.poo.lab12.entities.Business;
import com.poo.lab12.entities.Disability;
import com.poo.lab12.entities.Employee;
import com.poo.lab12.entities.Gender;
import com.poo.lab12.entities.Project;
import com.poo.lab12.entities.Religion;

public class BusinessReport {

    public static Map<Disability, List<Employee>> getEmployeesOnSameDisability(Business business) {

        return business.getEmployees()
                .stream()
                .collect(Collectors.groupingBy(Employee::getDisability));
    }

    public static int getNumberOfProjectsWorkedByCurrentEmployees(Business business) {

        Predicate<Project> isBusinessProject = str -> business.getProjects().contains(str);

        return  business.getProjects()
                .stream()
                .filter(isBusinessProject)
                .collect(Collectors.groupingBy(Project::getName))
                .size();
    }

    public static SortedSet getEmployeesMaxByReligionMaxByGenderSortedByCitySortedByAge(Business business) {

        return business.getEmployees()
                .stream()
                .collect(Collectors.groupingBy(Employee::getReligion))
                .entrySet().stream()
                .sorted(Map.Entry.comparingByValue(Comparator.comparing(value -> -value.size())))
                .collect(Collectors.toList())
                .get(0)
                .getValue()
                .stream()
                .collect(Collectors.groupingBy(Employee::getGender))
                .entrySet().stream()
                .sorted(Map.Entry.comparingByValue(Comparator.comparing(value -> -value.size())))
                .collect(Collectors.toList())
                .get(0)
                .getValue()
                .stream()
                .collect(Collectors.toCollection(
                        () -> new TreeSet<>(Comparator.comparing(Employee::getCity).thenComparing(Employee::getAge))
                ));
    }

    public static Religion getReligionOfEmployeesWithMostAccounts(Business business) {

        return business.getEmployees()
                .stream()
                .collect(Collectors.groupingBy(Employee::getReligion,
                        Collectors.summingInt(employer -> employer.getAccounts().size())))
                .entrySet().stream()
                .max(Map.Entry.comparingByValue())
                .map(Map.Entry::getKey).orElse(null);
    }

    public static Map<Project, Map<String, List<Employee>>> getEmployeesOnSameProjectAndCity(Business business) {

        return null;
    }

    public static Map<Religion, Map<Gender, List<Employee>>> getEmployeesOnSameReligionAndGender(Business business) {

        return business.getEmployees().stream()
                .collect(Collectors.groupingBy(Employee::getReligion, Collectors.groupingBy(Employee::getGender)));
    }
}
