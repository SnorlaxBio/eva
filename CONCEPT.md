EVENT LIB CONCEPT NOTE
======================

```c
struct object;
struct object_func;

typedef struct object object_t;
typedef struct object_func object_func_t;

struct object {
    objec_func_t * func;
    sync_t * sync;
};

struct object_func {
    object_t * (*rem)(object_t *);
};
```

위의 구조체는 기본 클래스의 골격이다.

## EVENT HANDLER

![Class Diagram Event Handler](/docs/assets/images/ClassDiagramEventHandler.jpg)

함수를 인터페이스 처럼 정의한 타입으로 이벤트 큐에서 POP 을 수행하면, ENGINE 객체의 멤버인 QUEUE 에 삽입된 이벤트가 출력되어져서 나온다. 

## EVENT ENGINE OFF

시그널만 보내고 끝이 난다.
엔진 상태가 RUNNING 상태이면, ...