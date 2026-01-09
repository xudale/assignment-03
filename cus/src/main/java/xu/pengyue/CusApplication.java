package xu.pengyue;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.scheduling.annotation.EnableScheduling;
import xu.pengyue.config.AppConfig;

@SpringBootApplication
@EnableScheduling
@EnableConfigurationProperties(AppConfig.class)
public class CusApplication {

    public static void main(String[] args) {
        SpringApplication.run(CusApplication.class, args);
    }
}
