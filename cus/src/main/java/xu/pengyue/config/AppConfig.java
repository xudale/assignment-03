package xu.pengyue.config;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;

@Configuration
@ConfigurationProperties(prefix = "app")
public class AppConfig {
    public Mqtt mqtt = new Mqtt();
    public Serial serial = new Serial();
    public Policy policy = new Policy();
    public History history = new History();

    public static class Mqtt {
        public String host;
        public String topicLevel;
    }
    public static class Serial {
        public int baud;
    }
    public static class Policy {
        public double l1;
        public double l2;
        public long t1Ms;
        public long t2Ms;
    }
    public static class History {
        public int n;
    }
}
