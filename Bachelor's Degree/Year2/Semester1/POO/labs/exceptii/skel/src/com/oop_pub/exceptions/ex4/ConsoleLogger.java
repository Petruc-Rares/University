package src.com.oop_pub.exceptions.ex4;

import java.util.EnumSet;

public class ConsoleLogger extends LoggerBase {
    ConsoleLogger() {
        super(EnumSet.allOf(LogLevel.class));
    }

    @Override
    protected void writeMessage(String message) {
        System.out.println("[Console] " + message);
    }
}
