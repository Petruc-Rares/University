package com.poo.lab12.reports;

import java.util.*;
import java.util.function.Function;
import java.util.function.Supplier;
import java.util.stream.Collectors;
import java.util.stream.DoubleStream;

import com.poo.lab12.entities.Account;
import com.poo.lab12.entities.Bank;
import com.poo.lab12.entities.Employee;
import com.poo.lab12.entities.Gender;
import com.poo.lab12.entities.Project;

public class BankReport {

    public static int getNumberOfCustomers(Bank bank) {
        // A customer is an Employee that works for a Business (client of the bank)

        return bank.getClients()
                .stream()
                .mapToInt(business -> business.getEmployees().size())
                .sum();
    }

    public static int getNumberOfAccounts(Bank bank) {
        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .mapToInt(employee -> employee.getAccounts().size())
                .sum();
    }

    public static SortedSet getCustomersSorted(Bank bank) {
        // Display the set of customers in alphabetical order
        Comparator<Employee> byName = Comparator.comparing(Employee::getName);

        Supplier<TreeSet<Employee>> employer = () -> new TreeSet<>(byName);

        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .collect(Collectors.toCollection(employer));
    }

    public static double getTotalSumInAccounts(Bank bank) {
        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .flatMap(employee -> employee.getAccounts().stream())
                .flatMapToDouble(account -> DoubleStream.of(account.getBalance()))
                .sum();
    }

    public static SortedSet getAccountsSortedBySum(Bank bank) {
        // The set of all accounts, ordered by current account balance

        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .flatMap(employee -> employee.getAccounts().stream())
                .collect(Collectors.toCollection(
                        () -> new TreeSet<>(Comparator.comparing(Account::getBalance))
                ));
    }

    public static Map<Employee, Collection<Account>> getCustomerAccounts(Bank bank) {
        // Return a map of all the customers with their respective accounts

        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .collect(Collectors.toMap(Function.identity(), Employee::getAccounts));
    }

    public static Map<String, List<Employee>> getCustomersByCity(Bank bank) {
        // Map all the customers to their respective cities

        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .collect(Collectors.groupingBy(Employee::getCity));
    }

    public static Project getProjectWorkedOnByMostCustomers(Bank bank) {

        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .flatMap(employee -> employee.getProjects().stream())
                .collect(Collectors.groupingBy(Function.identity(),
                        Collectors.counting()))
                .entrySet().stream()
                .max(Map.Entry.comparingByValue())
                .map(Map.Entry::getKey).orElse(null);
    }

    public static Gender getGenderWhoWorkedOnMostProjects(Bank bank) {
        
        return bank.getClients()
                .stream()
                .flatMap(business -> business.getEmployees().stream())
                .collect(Collectors.groupingBy(Employee::getGender,
                        Collectors.summingInt(employer -> employer.getProjects().size())))
                .entrySet().stream()
                .max(Map.Entry.comparingByValue())
                .map(Map.Entry::getKey).orElse(null);
    }
}
