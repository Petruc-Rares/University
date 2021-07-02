package queries;


import common.Constants;
import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import org.json.simple.JSONArray;

import java.io.IOException;

@SuppressWarnings("unchecked")
public class Query {

    private final JSONArray arrayResult;
    private final Input input;
    private final Writer fileWriter;

    public Query(final JSONArray arrayResult, final Input input, final Writer fileWriter) {
        this.arrayResult = arrayResult;
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current query
     * @param query represents the current query
     * @throws IOException in case of exceptions to reading / writing
     */
    public void solveQuery(final ActionInputData query) throws IOException {
        String type = query.getObjectType();
        String criteria = query.getCriteria();

        // for every type, also verify
        // the criteria to solve the appropriate query
        switch (type) {
            case Constants.ACTORS -> {
                switch (criteria) {
                    case Constants.AVERAGE -> {
                        ActorAverageQuery avgQuery = new ActorAverageQuery(input, fileWriter);
                        arrayResult.add(avgQuery.solveQuery(query));
                    }
                    case Constants.AWARDS -> {
                        ActorAwardsQuery awardQuery = new ActorAwardsQuery(input, fileWriter);
                        arrayResult.add(awardQuery.solveQuery(query));
                    }
                    case Constants.FILTER_DESCRIPTIONS -> {
                        ActorFDQuery fdQuery = new ActorFDQuery(input, fileWriter);
                        arrayResult.add(fdQuery.solveQuery(query));
                    }
                    default -> throw new IllegalStateException("Unexpected value: " + criteria);
                }
            }
            case Constants.MOVIES -> {
                switch (criteria) {
                    case Constants.FAVORITE -> {
                        MovieFavQuery movFavQuery = new MovieFavQuery(input, fileWriter);
                        arrayResult.add(movFavQuery.solveQuery(query));
                    }
                    case Constants.RATINGS -> {
                        MovieRatQuery movRatQuery = new MovieRatQuery(input, fileWriter);
                        arrayResult.add(movRatQuery.solveQuery(query));
                    }
                    case Constants.LONGEST -> {
                        MovieLongestQuery movLgstQuery = new MovieLongestQuery(input, fileWriter);
                        arrayResult.add(movLgstQuery.solveQuery(query));
                    }
                    case Constants.MOSTVIEWED -> {
                        MovieMstVwQuery movMVQuery = new MovieMstVwQuery(input, fileWriter);
                        arrayResult.add(movMVQuery.solveQuery(query));
                    }
                    default -> throw new IllegalStateException("Unexpected value: " + criteria);
                }
            }
            case Constants.SHOWS -> {
                switch (criteria) {
                    case Constants.FAVORITE -> {
                        ShowFavQuery showFavQuery = new ShowFavQuery(input, fileWriter);
                        arrayResult.add(showFavQuery.solveQuery(query));
                    }
                    case Constants.RATINGS -> {
                        ShowRatQuery showRatQuery = new ShowRatQuery(input, fileWriter);
                        arrayResult.add(showRatQuery.solveQuery(query));
                    }
                    case Constants.LONGEST -> {
                        ShowLongestQuery showLgstQuery = new ShowLongestQuery(input, fileWriter);
                        arrayResult.add(showLgstQuery.solveQuery(query));
                    }
                    case Constants.MOSTVIEWED -> {
                        ShowMstVwQuery showMVQuery = new ShowMstVwQuery(input, fileWriter);
                        arrayResult.add(showMVQuery.solveQuery(query));
                    }
                    default -> throw new IllegalStateException("Unexpected value: " + criteria);
                }
            }
            case Constants.USERS -> {
                if (Constants.NUM_RATINGS.equals(criteria)) {
                    ActorNrQuery actorNrQuery = new ActorNrQuery(input, fileWriter);
                    arrayResult.add(actorNrQuery.solveQuery(query));
                } else {
                    throw new IllegalStateException("Unexpected value: " + criteria);
                }
            }
            default -> throw new IllegalStateException("Unexpected value: " + type);
        }
    }
}
