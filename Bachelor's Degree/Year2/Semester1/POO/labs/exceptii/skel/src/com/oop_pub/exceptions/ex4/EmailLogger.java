package src.com.oop_pub.exceptions.ex4;

import java.util.EnumSet;

public class EmailLogger extends LoggerBase {
    EmailLogger() {
        super(EnumSet.of(LogLevel.FunctionalMessage, LogLevel.FunctionalError));
    }

    @Override
    protected void writeMessage(String message) {
        System.out.println("[Email] " + message);
    }
}
