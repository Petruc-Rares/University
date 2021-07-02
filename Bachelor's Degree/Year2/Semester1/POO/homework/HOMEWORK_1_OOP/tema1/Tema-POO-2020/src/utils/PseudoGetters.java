package utils;

import newfileio.Input;
import newfileio.MovieData;
import newfileio.SerialData;
import newfileio.UserData;

import java.util.List;

/**
 * The class contains static methods that helps with returning
 * instances for specific input and name
 */
public final class PseudoGetters {
    /**
     * for coding style
     */
    private PseudoGetters() {
    }


    /**
     * @param username represents the name from the action
     * @param input represents the input from main
     * @return an instance of UserData corresponding to username, given input
     */
    public static UserData getUser(final String username, final Input input) {
        List<UserData> users = input.getUsers();
        for (UserData user : users) {
            if (user.getUsername().equals(username)) {
                return user;
            }
        }
        return null;
    }

    /**
     * @param movieName represents the name of the movies
     * @param input represents the input from main
     * @return an instance of MovieData corresponding to movieName, given input
     */
    public static MovieData getMovie(final String movieName, final Input input) {
        List<MovieData> movies = input.getMovies();
        for (MovieData movie : movies) {
            if (movie.getTitle().equals(movieName)) {
                return movie;
            }
        }
        return null;
    }

    /**
     * @param serialName represents the name of the serial
     * @param input represents the input from main
     * @return an instance of SerialData corresponding to serialName, given input
     */
    public static SerialData getSerial(final String serialName, final Input input) {
        List<SerialData> serials = input.getSerials();
        for (SerialData serial : serials) {
            if (serial.getTitle().equals(serialName)) {
                return serial;
            }
        }
        return null;
    }

}
