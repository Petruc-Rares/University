import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Objects;
import java.util.PriorityQueue;
import java.util.Queue;

public class Adrese {

	private static class GraphPlatform {
		private HashMap<PlatformInstance, List<PlatformInstance>> relationshipPlatform;
		private Integer noInstances;

		public GraphPlatform() {
			this.relationshipPlatform = new HashMap<>();
		}

		public void addVertex(PlatformInstance vertex) {
			relationshipPlatform.put(vertex, new ArrayList<>());
		}

		public boolean hasVertex(PlatformInstance key) {
			return relationshipPlatform.containsKey(key);
		}

		public void addEdge(PlatformInstance s, PlatformInstance d) {
			relationshipPlatform.get(s).add(d);
			relationshipPlatform.get(d).add(s);
		}

		public void setNoInstances(Integer noInstances) {
			this.noInstances = noInstances;
		}

		public HashMap<PlatformInstance, List<PlatformInstance>> getRelationshipPlatform() {
			return relationshipPlatform;
		}
	}

	private static class PlatformInstance {
		String name;
		//  0 - userName, 1 - address
		int type;
		// this one will be used not only to differentiate between
		// people holding the same userName, but which might be different,
		// but also for arrays like visited.
		int id;

		public PlatformInstance(String name, int type, int id) {
			this.name = name;
			this.type = type;
			this.id = id;
		}

		@Override
		public boolean equals(Object o) {
			if (this == o) {
				return true;
			}
			if (o == null || getClass() != o.getClass()) {
				return false;
			}
			PlatformInstance that = (PlatformInstance) o;
			if ((type == 1) && (that.type == 1)) {
				return name.equals(that.name);
			}
			return type == that.type && id == that.id && Objects.equals(name, that.name);
		}

		@Override
		public int hashCode() {
			if (type == 0) {
				return Objects.hash(id);
			} else if (type == 1) {
				return Objects.hash(name);
			}
			return 0;
		}
	}

	public static GraphPlatform createGraphPlatfom() {
		MyScanner scanner = new MyScanner("adrese.in");
		GraphPlatform graphPlatform = new GraphPlatform();
		relationshipPlatform = graphPlatform.getRelationshipPlatform();

		int N = scanner.nextInt();

		HashMap<PlatformInstance, Integer> emails = new HashMap<>();

		int id = 0;
		// for each username, read mail addresses associated with it
		// and other data like, number of mails, etc.
		for (int crtUser = 0; crtUser < N; crtUser++) {
			String[] infoUser = scanner.nextLine().split(" ");

			String nameUser = infoUser[0];
			PlatformInstance user = new PlatformInstance(nameUser, 0, id);
			id++;
			graphPlatform.addVertex(user);


			int NoAddresses = Integer.parseInt(infoUser[1]);
			
			for (int i = 0; i < NoAddresses; i++) {
				PlatformInstance newMail = new PlatformInstance(scanner.next(), 1, id);
				if (!(graphPlatform.hasVertex(newMail))) {
					graphPlatform.addVertex(newMail);

					emails.put(newMail, id);
					id++;
				} else {
					newMail.id = emails.get(newMail);
				}
				graphPlatform.addEdge(user, newMail);
			}
		}

		graphPlatform.setNoInstances(id);
		return graphPlatform;
	}

	private static Boolean[] visited;

	static class ResultFormat {
		String userName;
		Integer noMails;
		PriorityQueue<String> mails;

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public Integer getNoMails() {
			return noMails;
		}

		public void setNoMails(Integer noMails) {
			this.noMails = noMails;
		}

		public PriorityQueue<String> getMails() {
			return mails;
		}

		public void setMails(PriorityQueue<String> mails) {
			this.mails = mails;
		}
	}

	public static ResultFormat bfs_util(PlatformInstance platformInstance) {

		PriorityQueue<String> emails = new PriorityQueue<>();
		String lowestName = BIGGESTNAME;

		Queue<PlatformInstance> queue = new LinkedList<>();
		queue.add(platformInstance);

		ResultFormat result = new ResultFormat();
		int no = 0;
		while (!(queue.isEmpty())) {
			PlatformInstance crtElem = queue.remove();

			if (crtElem.type == 0) {
				// case username
				if (lowestName.compareTo(crtElem.name) > 0) {
					lowestName = crtElem.name;
				}
			} else if (crtElem.type == 1) {
				// case mail
				emails.add(crtElem.name);
				no++;
			}

			List<PlatformInstance> succs = relationshipPlatform.get(crtElem);
			for (PlatformInstance succ : succs) {
				if (visited[succ.id] == null) {
					visited[succ.id] = true;
					queue.add(succ);
				}
			}
		}

		result.setUserName(lowestName);
		result.setMails(emails);
		result.setNoMails(no);

		return result;
	}

	static class ComparatorResult implements Comparator<ResultFormat> {
		@Override
		public int compare(ResultFormat a, ResultFormat b) {
			int diff = a.noMails - b.noMails;
			if (diff != 0) {
				return diff;
			}

			return a.userName.compareTo(b.userName);
		}
	}

	static HashMap<PlatformInstance, List<PlatformInstance>> relationshipPlatform;


	public static Integer bfs(GraphPlatform graphPlatform) {
		visited = new Boolean[graphPlatform.noInstances];

		List<ResultFormat> results = new ArrayList<>();

		Integer counts = 0;
		HashMap<PlatformInstance, List<PlatformInstance>> relationshipPlatform =
				graphPlatform.getRelationshipPlatform();
		for (PlatformInstance platformInstance: relationshipPlatform.keySet()) {
			if (visited[platformInstance.id] == null) {
				visited[platformInstance.id] = true;
				ResultFormat result = bfs_util(platformInstance);
				results.add(result);
				counts++;
			}
		}

		Collections.sort(results, new ComparatorResult());

		String crtMail;
		try {
			BufferedWriter bufWriter = new BufferedWriter(new FileWriter("adrese.out"));
			bufWriter.write(String.valueOf(counts));
			bufWriter.newLine();

			for (ResultFormat result: results) {
				bufWriter.write(result.userName + " " + result.noMails);
				bufWriter.newLine();
				PriorityQueue<String> mails = result.mails;
				while ((crtMail = mails.poll()) != null) {
					bufWriter.write(crtMail);
					bufWriter.newLine();
				}
			}


			bufWriter.close();
		} catch (IOException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}

		System.out.println(counts);
		return 1;
	}

	public static final String BIGGESTNAME = "ZZZZZZZZZZZZZZZZZZZZ";

	public static void main(String[] args) {
		GraphPlatform graphPlatform = createGraphPlatfom();
		bfs(graphPlatform);
	}

}
