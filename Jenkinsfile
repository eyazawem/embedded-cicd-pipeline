pipeline {
    agent any

    options {
        timestamps()
        timeout(time: 30, unit: 'MINUTES')
        disableConcurrentBuilds()
        buildDiscarder(logRotator(numToKeepStr: '10'))
    }

    environment {
        MIN_LINE_COVERAGE = '90'
    }

    stages {
        stage('Build reference image') {
            steps {
                sh 'docker build -t embedded-cicd-builder:24.04 .'
            }
        }
        stage('Check formatting') {
            steps {
                sh './scripts/format.sh --check'
            }
        }
        stage('Static analysis') {
            steps {
                sh './scripts/static_analysis.sh'
            }
        }
        stage('Build and test') {
            steps {
                sh './scripts/build_and_test.sh'
            }
        }
        stage('Coverage') {
            steps {
                sh './scripts/coverage.sh'
            }
        }
        stage('Runtime image') {
            steps {
                sh './scripts/build_runtime_image.sh'
            }
        }
    }

    post {
        always {
            archiveArtifacts artifacts: 'coverage/**', allowEmptyArchive: true
        }
        cleanup {
            deleteDir()
        }
    }
}
