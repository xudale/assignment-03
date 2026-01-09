package xu.pengyue.config;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@ConfigurationProperties(prefix = "app")
@Component
@Data
public class AppConfig {
    private Mqtt mqtt = new Mqtt();
    private Serial serial = new Serial();
    private Policy policy = new Policy();
    private History history = new History();

    @Data
    public static class Mqtt {
        private String host;
        private String topicLevel;
    }

    @Data
    public static class Serial {
        private int baud;
    }

    @Data
    public static class Policy {
        private double l1;
        private double l2;
        private long t1Ms;
        private long t2Ms;
    }

    @Data
    public static class History {
        private int n;
    }
}
