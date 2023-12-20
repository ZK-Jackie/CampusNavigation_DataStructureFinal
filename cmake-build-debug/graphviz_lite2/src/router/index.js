import Vue from 'vue'
import VueRouter from 'vue-router'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'about',
    component: () => import('../views/MainView.vue')
  }
]

const router = new VueRouter({
  routes
})

export default router
