package src.com.oop_pub.exceptions.ex2_3;

import java.util.Collection;
import java.util.Iterator;

// Alegerea fireasca sunt exceptiile unchecked deoarece exceptiile
// noastre extind RuntimeException, insa eu am implementat si try-catchurile
// pentru a verifica corectitudinea implementarilor din clasa My_Calculator
// deci scopul este strict didactic.

public class My_Calculator implements Calculator {
    @Override
    public Double add(Double nr1, Double nr2) throws NullParameterException,
            UnderflowException, OverflowException {
        if ((nr1 == null) || (nr2 == null)) throw new NullParameterException();

        Double sum = nr1 + nr2;

        if (sum.equals(Double.POSITIVE_INFINITY)) throw new OverflowException();
        else if (sum.equals(Double.NEGATIVE_INFINITY)) throw new UnderflowException();
        else return sum;
    }

    @Override
    public Double divide(Double nr1, Double nr2) throws NullParameterException,
            UnderflowException, OverflowException {
        if ((nr1 == null) || (nr2 == null)) throw new NullParameterException();

        Double divideResult = nr1 / nr2;

        if (divideResult.equals(Double.POSITIVE_INFINITY)) throw new OverflowException();
        else if (divideResult.equals(Double.NEGATIVE_INFINITY)) throw new UnderflowException();
        else return divideResult;
    }

    @Override
    public Double average(Collection<Double> numbers) throws NullParameterException {
        if (numbers == null) throw new NullParameterException();
        Integer numbersSize = numbers.size();

        if (numbersSize % 2 != 0) {
            numbers.add((double) 0);
        }

        Double sum = (double) 0;
        Iterator<Double> it = numbers.iterator();

        while (it.hasNext()) {
            Double number1 = it.next();
            Double number2 = it.next();

            sum += add(number1, number2);
        }
        return divide(sum, Double.valueOf(numbersSize));

    }
}
