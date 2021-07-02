package usefuldatastructures;

import java.util.Objects;

public class VertexFamily {
    int id;

    public VertexFamily(int id) {
        this.id = id;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        VertexFamily that = (VertexFamily) o;
        return id == that.id;
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }

    public int getId() {
        return id;
    }

    @Override
    public String toString() {
        return "VertexFamily{" +
                "id=" + id +
                '}';
    }
}
