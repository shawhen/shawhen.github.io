# Bug回忆录：Gradle/Spring多项目拆分

## 前言
最近我们将之前的一个python技术栈的项目完全转移到了java技术栈，转移后项目日趋庞大，便着手进行项目拆分。

## 拆分步骤
我们将共用的与具体业务无关的部分拆为common，另外两个业务一个是主干业务officep，一个是附加业务findd，officep和findd都依赖common，findd同时还依赖officep。拆分之后目录结构看起来像是这样：

- common
	
	|-- build.gradle
	
	|-- src/main/java/com/uranome/fbs/common/

- officep

	|-- build.gralde
	
	|-- src/main/java/com/uranome/fbs/officep/
	
	|-- src/main/java/com/uranome/fbs/officep/persistance/UserRepository.java

- findd

	|-- build.gradle
	
	|-- src/main/java/
	
	|-- src/main/java/com/uranome/fbs/findd/service/EvaluationService.java
	
	
- build.gradle

- settings.gradle

## Bug的来临
build.gradle和settings.gradle文件的编写直接参照官方doc[`Authoring Multi-Project Builds`](https://docs.gradle.org/current/userguide/multi_project_builds.html)。编译顺利完成，officep项目也能顺利启动，但是在启动findd项目时报错。大意是EvaluationService装配UserRepository失败，找不到UserRepository这个Bean。bug来得总是如狂风暴雨一般猛烈，反复确认UserRepository加了`@Repository`这个注解之后bug依然存在。

## Bug的尝试解决
### 1. 修改ComponentScan
将`com.uranome.fbs.officep.persistance`加入到findd的ComponentScan，测试问题依然存在。

### 2. 添加EntityScan
经过一番google之后添加注解：

    @EntityScan({
        "com.uranome.fbs.findd",
        "com.uranome.fbs.officep.persistance",
    })
    
这次抛出的异常和之前有所不同，但依然是起不来。

## 进行思考
根据我们现有的知识可以推断这个bug的产生原因应该bean自动装配失败，而就目前来看装配失败无外乎两个原因:

1. bean所在类未被扫描到。
2. bean所在类被扫描到了，但是由于某种原因导致没有创建bean。

经过长达2天的google之后bug依然没有解决，我开始意识到解决这个bug需要对spring的DI整个流程有比较深入的了解。

之后就是clone了SpringBoot的仓库，循着`run`一路读代码+调试，在`org.springframework.context.annotation.ConfigurationClassParser`的`doProcessConfigurationClass`方法中看到了`com.uranome.fbs.officep.persistance`这个包，可知原因1可被排除。接下来验证原因2即可，所有的singleton beans都在`ApplicationContext`持有的`DefaultListableBeanFactory.preInstantiateSingletons`中创建(如果你看不懂这一段我在讲啥，可以参考一篇Spring源码解读的文章，或者自己看看源代码)。可以看到之所以UserRepository这个bean没有被创建，因为在`isAbstract`的判断中被排除了，意思就是说UserRepository是`abstract`的。回想一下Repository在Spring JPA中都是`interface`声明的，不是`abstract`才怪呢。

了解了bug产生的原因之后针对于此在google上又搜索了一番，总算得到了解决方案。`Repository`需要特殊的处理才能被创建为bean，即添加`@EnableJpaRepositories`注解。

## Bug的解决
最终得到的代码大概是这样：

src/main/java/com/uranome/fbs/findd/App.java

    @SpringBootApplication
    @ComponentScan({"com.uranome.fbs.common", "com.uranome.fbs.findd",
     "com.uranome.fbs.officep.persistance", "com.uranome.fbs.officep.service"})
    @EnableJpaRepositories({"com.uranome.fbs.findd", "com.uranome.fbs.officep.persistance"})
    @EntityScan({"com.uranome.fbs.findd", "com.uranome.fbs.officep.persistance"})
    public class App
