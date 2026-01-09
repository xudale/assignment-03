package xu.pengyue;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import xu.pengyue.config.AppConfig;

@SpringBootApplication
public class CusApplication {

    public static void main(String[] args) {
        SpringApplication.run(CusApplication.class, args);
    }
}