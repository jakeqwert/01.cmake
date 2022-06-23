pipeline {
  agent none
  options {  //Блок опций может быть использован для настройки выполнения пайплайна или же отдельного шага
    skipStagesAfterUnstable() //заставит Jenkins сразу прервать билд, eсли тесты были провалены. Поведение по умолчанию предусматривает установку статуса билда в UNSTABLE и продолжение выполнения.
    skipDefaultCheckout()     //отключает автоматический чекаут репозитория проекта. Дефолтно Jenkins делает force чекаут репозитория для каждого шага с собственным агентом
                              // (в нашем случае stage (Prepare Checkout, Push images и Trigger Kubernetes). То есть по сути затирает все изменения. Это может быть полезно при
                              // использовании пайплайна с несколькими различными образами. Однако нам нажно получить исходники с репозитория только единожды — на шаге Build.
                              // Применив опцию skipDefaultCheckout, мы получаем возможность произвести чекаут вручную. Также стоит заметить, что Jenkins будет автоматически
                              // переносить артефакты между шагами. Так, например, скомпилированные исходники из шага Build будут полностью доступны в шаге Test.
  }
  environment {  //Переменные среды могут быть также объявлены как для всего пайплайна, так и для отдельного шага.
//    IMAGE_BASE = 'anshelen/microservices-backend'
//    IMAGE_TAG = "v$BUILD_NUMBER"
//    IMAGE_NAME = "${env.IMAGE_BASE}:${env.IMAGE_TAG}"
//    IMAGE_NAME_LATEST = "${env.IMAGE_BASE}:latest"
//    DOCKERFILE_NAME = "Dockerfile-packaged"     //имя файла докера и имена создаваемых Docker-образов
  }
  stages {
    stage("Prepare container") {
      agent {
        docker {
//           image 'openjdk:11.0.5-slim'
//           args '-v $HOME/.m2:/root/.m2'
             image 'ubuntu:bionic'
        }
      }
      stages {
        stage('Prepare') {
          steps {
            checkout scm  //чекаут репозитория  Адрес репозитория указан при настройке пайплайна в Jenkins
      //             sh './mvnw compile'   // bash-команду на компиляцию проекта.
            sh 'apt-get update && apt-get -y install file build-essential cmake libboost-test-dev libspdlog-dev'

          }
        }
        stage('Build') {
          steps {
            checkout scm  //чекаут репозитория  Адрес репозитория указан при настройке пайплайна в Jenkins
//             sh './mvnw compile'   // bash-команду на компиляцию проекта.
            sh """
                - cmake .
                - cmake --build .
                - cmake --build . --target test
                - cmake --build . --target package
            """
          }
        }

        stage('Deploy') {
          steps {
            sh 'ls -la'
            sh 'echo "CI_PIPELINE_ID = ${CI_PIPELINE_ID}"'
            sh 'curl -f -T cmake-1.1.${CI_PIPELINE_ID}-Linux.deb -ueliseevz@mail.ru:${BINTRAY_API_KEY} -XPUT "https://jakekobra.jfrog.io/artifactory/study-cpp/homework/11/cmake-1.1.${CI_PIPELINE_ID}-Linux.deb;deb.distribution=xenial;deb.component=main;deb.architecture=amd64;publish=1"'
          }
        }

//         stage('Test') {
//           steps {
//             sh './mvnw test'
//             junit '**/target/surefire-reports/TEST-*.xml'  // мы указываем Jenkins'у на файл с результатами тестов. Это позволит их отобразить прямо в веб-интерфейсе.
//           }
//         }

//         stage('Package') {
//           steps {
//             sh './mvnw package -DskipTests'  //генерируем jar-архив с нашим приложением.
//           }
//         }
      }
    }


//     stage('Push images') {
//       agent any                  // на любом доступном агенте
//       when {
//         branch 'master'          // только для коммитов в мастер-ветку
//       }
//       steps {
//         script {
//           def dockerImage = docker.build("${env.IMAGE_NAME}", "-f ${env.DOCKERFILE_NAME} .")
//           docker.withRegistry('', 'dockerhub-creds') {
//             dockerImage.push()
//             dockerImage.push("latest")
//           }
//           echo "Pushed Docker Image: ${env.IMAGE_NAME}"
//         }
//         sh "docker rmi ${env.IMAGE_NAME} ${env.IMAGE_NAME_LATEST}"
//       }
//     }

//     stage('Trigger kubernetes') {
//       agent any
//       when {
//         branch 'master'
//       }
//       steps {
//         withKubeConfig([credentialsId: 'kubernetes-creds', serverUrl: "${CLUSTER_URL}", namespace: "${CLUSTER_NAMESPACE}"]) {
//           sh "helm upgrade ${HELM_PROJECT} ${HELM_CHART} --reuse-values --set backend.image.tag=${env.IMAGE_TAG}"
//         }
//       }

    }

  }

}