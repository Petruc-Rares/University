package src.com.oop_pub.exceptions.ex2_3;

import java.util.ArrayList;
import java.util.List;

public class MainEx2 {
    public static void main(String[] args) {
        Calculator calculator = new My_Calculator();

        System.out.println(calculator.add(2d, 3d));
        System.out.println(calculator.divide(9d, 4d));
        System.out.println(calculator.average(List.of(1d, 2d, 3d, 4d)));

        try {
            System.out.println(calculator.add(5d, null));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add null to something else");
        } catch (Calculator.OverflowException e) {
            System.out.println("the sum exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the sum exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.add(null, 3d));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add null to something else");
        } catch (Calculator.OverflowException e) {
            System.out.println("the sum exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the sum exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.add(Double.POSITIVE_INFINITY, 1d));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add null to something else");
        } catch (Calculator.OverflowException e) {
            System.out.println("the sum exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the sum exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.add(Double.NEGATIVE_INFINITY, -1d));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add null to something else");
        } catch (Calculator.OverflowException e) {
            System.out.println("the sum exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the sum exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.divide(null, 4d));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't divide null or divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.divide(5d, null));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't divide null or divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }


        try {
            System.out.println(calculator.divide(3d, 0d));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't divide null or divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.divide(-33d, 0d));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't divide null or divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.average(List.of(5d, Double.NEGATIVE_INFINITY, 3d, -9d)));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add/divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }

        try {
            System.out.println(calculator.average(List.of(10d, Double.POSITIVE_INFINITY, 3d, -9d)));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add/divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }

        List<Double> list = new ArrayList<>();
        list.add(null);
        list.add(2d);
        list.add(5d);

        try {
            System.out.println(calculator.average(list));
        } catch (Calculator.NullParameterException e) {
            System.out.println("can't add/divide by null");
        } catch (Calculator.OverflowException e) {
            System.out.println("the result exceeded the internal representation for positive numbers");
        } catch (Calculator.UnderflowException e) {
            System.out.println("the result exceeded the internal representation for negative numbers");
        }

    }
}
