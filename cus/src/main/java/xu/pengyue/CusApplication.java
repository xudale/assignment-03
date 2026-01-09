package xu.pengyue;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling
public class CusApplication {

    public static void main(String[] args) {
        SpringApplication.run(CusApplication.class, args);
    }
}
